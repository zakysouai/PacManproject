#include "logic/World.h"

#include "logic/utils/Random.h"
#include "logic/utils/Stopwatch.h"
#include "logic/entities/Ghost.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

namespace pacman {

World::World(AbstractFactory* factory)
    : factory(factory) {
}
void World::update(float deltaTime) {
    score.updateComboTimer(deltaTime);

    if (pacman) {
        updatePacManWithCollisions(deltaTime);
    }

    // Update ghosts
    for (auto& ghost : ghosts) {
        ghost->update(deltaTime);
    }

    handleCollisions();

    if (isLevelComplete()) {
        Event event;
        event.type = EventType::LEVEL_CLEARED;
        event.value = 500 * currentLevel;
        score.onNotify(event);
    }
}

bool World::wouldCollideWithWall(const Position& pos, float radius, const Ghost* ghost) const {
    BoundingBox testBox(
        pos.x - radius,
        pos.y - radius,
        radius * 2.0f,
        radius * 2.0f
    );

    // Check normale muren
    for (const auto& wall : walls) {
        if (testBox.intersects(wall->getBoundingBox())) {
            return true;
        }
    }

    // ✅ SIMPEL: als ghost door deur is → deur = wall
    if (ghost && ghost->hasPassedThroughDoor() && hasDoor) {
        auto ghostGrid = worldToGrid(pos);
        if (ghostGrid.row == doorGridPos.row && ghostGrid.col == doorGridPos.col) {
            return true;  // Deur is wall voor deze ghost
        }
    }

    return false;
}

bool World::canMoveInDirection(const Position& pos, Direction dir, float radius, const Ghost* ghost) const {
    Position dirVector = getDirectionVector(dir);
    const float TEST_DISTANCE = 0.1f;
    Position testPos = Position(
        pos.x + dirVector.x * TEST_DISTANCE,
        pos.y + dirVector.y * TEST_DISTANCE
    );

    BoundingBox testBox(
        testPos.x - radius,
        testPos.y - radius,
        radius * 2.0f,
        radius * 2.0f
    );

    // Check normale muren
    for (const auto& wall : walls) {
        if (testBox.intersects(wall->getBoundingBox())) {
            return false;
        }
    }

    // ✅ SIMPEL: als ghost door deur is → deur = wall
    if (ghost && ghost->hasPassedThroughDoor() && hasDoor) {
        auto testGrid = worldToGrid(testPos);
        if (testGrid.row == doorGridPos.row && testGrid.col == doorGridPos.col) {
            return false;  // Deur blokkeert deze ghost
        }
    }

    return true;
}
bool World::isAtIntersection(const Position& pos, Direction currentDir, float radius) const {
    // An intersection is where you can move in a direction perpendicular to your current movement

    if (currentDir == Direction::NONE) {
        return true;  // Not moving yet, so we can go any direction
    }

    // Get perpendicular directions
    std::vector<Direction> perpendicular;
    if (currentDir == Direction::UP || currentDir == Direction::DOWN) {
        perpendicular = {Direction::LEFT, Direction::RIGHT};
    } else {
        perpendicular = {Direction::UP, Direction::DOWN};
    }

    // Check if we can move in at least one perpendicular direction
    for (Direction dir : perpendicular) {
        if (canMoveInDirection(pos, dir, radius)) {
            return true;
        }
    }

    return false;
}

void World::updatePacManWithCollisions(float deltaTime) {
    Direction currentDir = pacman->getDirection();
    Direction nextDir = pacman->getNextDirection();

    // If nextDirection is different from currentDirection, try to switch at intersection
    if (nextDir != Direction::NONE && nextDir != currentDir) {
        // Check if we're at an intersection or if the next direction is opposite to current
        bool isOpposite = (
            (currentDir == Direction::UP && nextDir == Direction::DOWN) ||
            (currentDir == Direction::DOWN && nextDir == Direction::UP) ||
            (currentDir == Direction::LEFT && nextDir == Direction::RIGHT) ||
            (currentDir == Direction::RIGHT && nextDir == Direction::LEFT)
        );

        // Allow direction change if:
        // 1. At an intersection (can turn perpendicular)
        // 2. Opposite direction (instant 180° turn)
        // 3. Not moving yet (just started)
        Position currentPos = pacman->getPosition();
        float radius = pacman->getCollisionRadius();

        if (isOpposite || currentDir == Direction::NONE ||
            isAtIntersection(currentPos, currentDir, radius)) {

            // Try to move in the next direction
            if (canMoveInDirection(currentPos, nextDir, radius)) {
                // Direction change is possible!
                pacman->tryChangeDirection(nextDir);
                currentDir = nextDir;  // Update for the movement below
            }
        }
    }

    // Now move in the current direction (which may have just been updated)
    if (currentDir == Direction::NONE) {
        return;  // Not moving
    }

    Position currentPos = pacman->getPosition();
    Position dirVector = getDirectionVector(currentDir);
    float speed = pacman->getSpeed();

    // Calculate full movement
    Position movement = dirVector * speed * deltaTime;

    // Per-axis collision detection with AABB
    if (currentDir == Direction::LEFT || currentDir == Direction::RIGHT) {
        // Horizontal movement - try X-axis first
        Position testPos = Position(currentPos.x + movement.x, currentPos.y);

        // Temporarily set position to test collision
        Position originalPos = pacman->getPosition();
        pacman->setPosition(testPos);

        bool collisionX = false;
        for (const auto& wall : walls) {
            if (pacman->intersects(*wall)) {
                collisionX = true;
                break;
            }
        }

        if (collisionX) {
            // Restore original position - can't move in X
            pacman->setPosition(originalPos);
        }

    } else if (currentDir == Direction::UP || currentDir == Direction::DOWN) {
        // Vertical movement - try Y-axis first
        Position testPos = Position(currentPos.x, currentPos.y + movement.y);

        Position originalPos = pacman->getPosition();
        pacman->setPosition(testPos);

        bool collisionY = false;
        for (const auto& wall : walls) {
            if (pacman->intersects(*wall)) {
                collisionY = true;
                break;
            }
        }

        if (collisionY) {
            // Restore original position - can't move in Y
            pacman->setPosition(originalPos);
        }
    }
}

bool World::isPositionBlocked(const Position& pos, float radius) const {
    // Check collision with all walls
    for (const auto& wall : walls) {
        float distance = pos.distance(wall->getPosition());
        // If distance between centers is less than sum of radii, there's a collision
        if (distance < radius + wall->getCollisionRadius()) {
            return true;
        }
    }
    return false;
}

bool World::checkWallCollision(const Position& pos, float radius) const {
    // Check if position would collide with any wall
    for (const auto& wall : walls) {
        float distance = pos.distance(wall->getPosition());
        float minDistance = radius + wall->getCollisionRadius();

        if (distance < minDistance) {
            return true;  // Collision detected!
        }
    }
    return false;  // No collision, movement is safe
}

void World::handleCollisions() {
    if (!pacman) return;

    // Check coin collisions
    for (auto& coin : coins) {
        if (!coin->isCollected() && pacman->intersects(*coin)) {
            coin->collect();
        }
    }

    // Check fruit collisions
    for (auto& fruit : fruits) {
        if (!fruit->isCollected() && pacman->intersects(*fruit)) {
            fruit->collect();
        }
    }
}

bool World::isLevelComplete() const {
    // Level complete when all coins and fruits collected
    for (const auto& coin : coins) {
        if (!coin->isCollected()) return false;
    }
    for (const auto& fruit : fruits) {
        if (!fruit->isCollected()) return false;
    }
    return true;
}

bool World::isGameOver() const {
    return pacman && !pacman->isAlive();
}

void World::nextLevel() {
    currentLevel++;
    // Load same map, but harder
    applyDifficultyScaling();
}

void World::reset() {
    // Reset PacMan to his original spawn position from the map
    if (pacman) {
        pacman->reset(pacmanSpawnPosition);
        std::cout << "Reset PacMan to spawn position ("
                  << pacmanSpawnPosition.x << ", "
                  << pacmanSpawnPosition.y << ")" << std::endl;
    }
}

void World::applyDifficultyScaling() {
}

Position World::gridToWorld(int row, int col, int totalRows, int totalCols) const {
    // Convert grid coordinates to normalized world coordinates
    // We need to preserve the aspect ratio of the map

    // Calculate aspect ratio
    float mapAspectRatio = static_cast<float>(totalCols) / static_cast<float>(totalRows);

    // Determine the world bounds based on aspect ratio
    // We'll keep the height at [-1, 1] and adjust the width accordingly
    float worldWidth = mapAspectRatio;  // e.g., 20/11 = 1.82
    float worldHeight = 1.0f;

    // Convert grid position to world position
    // Add 0.5 to center the tile in its grid cell
    float x = -worldWidth + (2.0f * worldWidth * (col + 0.5f) / totalCols);
    float y = -worldHeight + (2.0f * worldHeight * (row + 0.5f) / totalRows);

    return Position(x, y);
}

void World::loadLevel(const std::string& mapFile) {
    parseMap(mapFile);
}

void World::parseMap(const std::string& mapFile) {
    std::ifstream file(mapFile);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open map file: " + mapFile);
    }

    // Read all lines from the file
    std::vector<std::string> mapData;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            mapData.push_back(line);
            std::cout << "Map line " << mapData.size() << ": " << line << std::endl;
        }
    }
    file.close();

    if (mapData.empty()) {
        throw std::runtime_error("Map file is empty: " + mapFile);
    }

    std::cout << "Loaded map with " << mapData.size() << " rows" << std::endl;

    // Spawn entities based on map data
    spawnEntities(mapData);
}

void World::spawnEntities(const std::vector<std::string>& mapData) {
    int totalRows = mapData.size();
    int totalCols = 0;

    // Find the maximum number of columns
    for (const auto& row : mapData) {
        if (static_cast<int>(row.length()) > totalCols) {
            totalCols = row.length();
        }
    }

    mapRows = totalRows;
    mapCols = totalCols;

    std::cout << "Map size: " << totalRows << "x" << totalCols << std::endl;

    // Clear existing entities
    walls.clear();
    coins.clear();
    fruits.clear();
    ghosts.clear();

    Position pacmanSpawnPos(0, 0);
    bool pacmanSpawned = false;
    int wallCount = 0;
    int coinCount = 0;

    // Ghost spawn tracking
    bool redSpawned = false;
    bool pinkSpawned = false;
    bool cyanSpawned = false;
    bool orangeSpawned = false;
    Position redPos(0, 0);
    Position pinkPos(0, 0);
    Position cyanPos(0, 0);
    Position orangePos(0, 0);

    // Parse the map and create entities
    for (int row = 0; row < totalRows; ++row) {
        for (int col = 0; col < totalCols; ++col) {
            if (col >= static_cast<int>(mapData[row].length())) continue;

            char tile = mapData[row][col];
            Position worldPos = gridToWorld(row, col, totalRows, totalCols);

            switch (tile) {
            case 'x':  // Wall
            case 'X':
                walls.push_back(factory->createWall(worldPos));
                wallCount++;
                break;

            case ' ':  // Empty space with coin
                coins.push_back(factory->createCoin(worldPos));
                coinCount++;
                break;

            case 'p':  // PacMan spawn position
            case 'P':
                pacmanSpawnPos = worldPos;
                pacmanSpawned = true;
                std::cout << "Found PacMan spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'c':  // Coin/Fruit (power pellet)
            case 'C':
                fruits.push_back(factory->createFruit(worldPos));
                std::cout << "Found Fruit spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'r':  // Red ghost
            case 'R':
                redPos = worldPos;
                redSpawned = true;
                std::cout << "Found Red ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'i':  // Pink ghost (i for pInk)
            case 'I':
                pinkPos = worldPos;
                pinkSpawned = true;
                std::cout << "Found Pink ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'b':  // Blue/Cyan ghost
            case 'B':
                cyanPos = worldPos;
                cyanSpawned = true;
                std::cout << "Found Cyan ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'o':  // Orange ghost
            case 'O':
                orangePos = worldPos;
                orangeSpawned = true;
                std::cout << "Found Orange ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'd':  // Door
            case 'D':
                doorPosition = worldPos;
                doorGridPos = {row, col};
                hasDoor = true;
                std::cout << "Found Door at grid(" << row << "," << col << ")" << std::endl;
                break;

            default:
                // Unknown character, ignore
                break;
            }
        }
    }

    std::cout << "Created " << wallCount << " walls and " << coinCount << " coins" << std::endl;

    // Create PacMan if specified in map
    if (pacmanSpawned) {
        pacmanSpawnPosition = pacmanSpawnPos;
        pacman = factory->createPacMan(pacmanSpawnPos);
        pacman->attach(&score);
        std::cout << "Created PacMan at (" << pacmanSpawnPos.x << ", " << pacmanSpawnPos.y << ")" << std::endl;
    } else {
        std::cout << "WARNING: No PacMan ('p' or 'P') found in map - no PacMan spawned!" << std::endl;
    }

    // Create ghosts via FACTORY
    if (redSpawned) {
        auto ghost = factory->createGhost(redPos, GhostColor::RED);
        ghost->setWorld(this);
        ghosts.push_back(std::move(ghost));
        std::cout << "Created RED ghost at (" << redPos.x << ", " << redPos.y << ")" << std::endl;
    }

    if (pinkSpawned) {
        auto ghost = factory->createGhost(pinkPos, GhostColor::PINK);
        ghost->setWorld(this);
        ghosts.push_back(std::move(ghost));
        std::cout << "Created PINK ghost at (" << pinkPos.x << ", " << pinkPos.y << ")" << std::endl;
    }

    if (cyanSpawned) {
        auto ghost = factory->createGhost(cyanPos, GhostColor::BLUE);
        ghost->setWorld(this);
        ghosts.push_back(std::move(ghost));
        std::cout << "Created CYAN ghost at (" << cyanPos.x << ", " << cyanPos.y << ")" << std::endl;
    }

    if (orangeSpawned) {
        auto ghost = factory->createGhost(orangePos, GhostColor::ORANGE);
        ghost->setWorld(this);
        ghosts.push_back(std::move(ghost));
        std::cout << "Created ORANGE ghost at (" << orangePos.x << ", " << orangePos.y << ")" << std::endl;
    }

    std::cout << "Created " << fruits.size() << " fruits from map" << std::endl;
    std::cout << "Created " << ghosts.size() << " ghosts from map" << std::endl;
    std::cout << "Map loading complete!" << std::endl;
}

bool World::isDoorPosition(const Position& pos) const {
    if (!hasDoor) return false;
    return pos.distance(doorPosition) < 0.05f;  // Small threshold
}

bool World::isDoorBlockingEntity(const Ghost* ghost, const Position& testPos) const {
    if (!isDoorPosition(testPos)) return false;

    // Door blocks ghost if it already passed through
    return ghost && ghost->hasPassedThroughDoor();
}

bool World::isDoorBlockedFor(const Position& pos, const Ghost* ghost) const {
    if (!hasDoor) return false;
    if (!ghost) return false;  // Deur blokkeert PacMan niet

    // ✅ Deur is ALLEEN geblokkeerd als:
    // 1. Deze ghost er AL door is gegaan
    // 2. EN we checken of ghost TERUG door de deur wil
    if (!ghost->hasPassedThroughDoor()) {
        return false;  // Ghost mag er nog door
    }

    // Ghost is er al door - check of hij bij de deur is
    return pos.distance(doorPosition) < 0.15f;
}

World::GridPosition World::worldToGrid(const Position& worldPos) const {
    float worldWidth = static_cast<float>(mapCols) / static_cast<float>(mapRows);
    float worldHeight = 1.0f;

    // Inverse van gridToWorld
    int col = static_cast<int>((worldPos.x + worldWidth) / (2.0f * worldWidth) * mapCols);
    int row = static_cast<int>((worldPos.y + worldHeight) / (2.0f * worldHeight) * mapRows);

    return {row, col};
}


} // namespace pacman