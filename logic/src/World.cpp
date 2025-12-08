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
    if (deathAnimationPlaying) {
        deathAnimationTimer += deltaTime;

        // Only update PacMan for animation
        if (pacman) {
            pacman->update(deltaTime);
        }

        if (deathAnimationTimer >= DEATH_ANIMATION_DURATION) {
            deathAnimationPlaying = false;
            deathAnimationTimer = 0.0f;

            // Reset
            pacman->reset(pacmanSpawnPosition);
            for (auto& g : ghosts) {
                g->respawn();
            }
        }
        return;
    }

    score.updateComboTimer(deltaTime);

    if (pacman) {
        updatePacManWithCollisions(deltaTime);
        checkWraparound(pacman.get());
        pacman->update(deltaTime);  // ✅ TOEVOEGEN - stuurt ENTITY_UPDATED
    }

    for (auto& ghost : ghosts) {
        ghost->update(deltaTime);
        checkWraparound(ghost.get());
    }

    // ✅ TOEVOEGEN - sturen ENTITY_UPDATED events
    for (auto& coin : coins) {
        coin->update(deltaTime);
    }

    for (auto& fruit : fruits) {
        fruit->update(deltaTime);
    }

    for (auto& wall : walls) {
        wall->update(deltaTime);
    }

    handleCollisions();

    if (isLevelComplete()) {
        Event event;
        event.type = EventType::LEVEL_CLEARED;
        event.value = 500 * currentLevel;
        score.onNotify(event);
    }
}

void World::checkWraparound(EntityModel* entity) {
    if (!entity) return;

    // ✅ BEREKEN WORLD BOUNDS
    float mapAspectRatio = static_cast<float>(mapCols) / static_cast<float>(mapRows);
    float worldWidth = mapAspectRatio;

    const float THRESHOLD = 0.05f;

    Position pos = entity->getPosition();

    // ✅ RECHTS NAAR LINKS
    if (pos.x > worldWidth - THRESHOLD) {
        pos.x = -worldWidth + THRESHOLD;
        entity->setPosition(pos);
    }
    // ✅ LINKS NAAR RECHTS
    else if (pos.x < -worldWidth + THRESHOLD) {
        pos.x = worldWidth - THRESHOLD;
        entity->setPosition(pos);
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

    // ✅ DEUR: PacMan altijd blokkeren
    if (hasDoor) {
        auto testGrid = worldToGrid(pos);
        if (testGrid.row == doorGridPos.row && testGrid.col == doorGridPos.col) {
            if (!ghost) return true;  // PacMan → blocked
            return ghost->hasPassedThroughDoor();  // Ghost → blocked na passage
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

    // ✅ DEUR: PacMan altijd blokkeren
    if (hasDoor) {
        auto testGrid = worldToGrid(testPos);
        if (testGrid.row == doorGridPos.row && testGrid.col == doorGridPos.col) {
            if (!ghost) return false;  // PacMan → blocked
            return !ghost->hasPassedThroughDoor();  // Ghost → OK tot passage
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

    // Try direction change at intersections/opposite
    if (nextDir != Direction::NONE && nextDir != currentDir) {
        bool isOpposite = (
            (currentDir == Direction::UP && nextDir == Direction::DOWN) ||
            (currentDir == Direction::DOWN && nextDir == Direction::UP) ||
            (currentDir == Direction::LEFT && nextDir == Direction::RIGHT) ||
            (currentDir == Direction::RIGHT && nextDir == Direction::LEFT)
        );

        if (isOpposite || currentDir == Direction::NONE ||
            isAtIntersection(pacman->getPosition(), currentDir, pacman->getCollisionRadius())) {

            if (canMoveInDirection(pacman->getPosition(), nextDir, pacman->getCollisionRadius())) {
                pacman->tryChangeDirection(nextDir);
                currentDir = nextDir;
            }
        }
    }

    if (currentDir == Direction::NONE) return;

    // Calculate movement
    Position dirVector = getDirectionVector(currentDir);
    float speed = pacman->getSpeed();
    Position movement = dirVector * speed * deltaTime;

    // ✅ NIEUWE AANPAK: Test collision ZONDER position te wijzigen
    Position currentPos = pacman->getPosition();
    Position testPos = currentPos + movement;

    // Check collision at target position
    BoundingBox testBox(
        testPos.x - pacman->getCollisionRadius(),
        testPos.y - pacman->getCollisionRadius(),
        pacman->getCollisionRadius() * 2.0f,
        pacman->getCollisionRadius() * 2.0f
    );

    bool collision = false;
    for (const auto& wall : walls) {
        if (testBox.intersects(wall->getBoundingBox())) {
            collision = true;
            break;
        }
    }

    if (!collision) {
        // No collision - move freely
        pacman->setPosition(testPos);
    } else {
        // ✅ SLIDING: Try per-axis movement
        if (currentDir == Direction::LEFT || currentDir == Direction::RIGHT) {
            // Try X-only movement
            Position xOnlyPos = Position(currentPos.x + movement.x, currentPos.y);
            BoundingBox xBox(
                xOnlyPos.x - pacman->getCollisionRadius(),
                xOnlyPos.y - pacman->getCollisionRadius(),
                pacman->getCollisionRadius() * 2.0f,
                pacman->getCollisionRadius() * 2.0f
            );

            bool xCollision = false;
            for (const auto& wall : walls) {
                if (xBox.intersects(wall->getBoundingBox())) {
                    xCollision = true;
                    break;
                }
            }

            if (!xCollision) {
                pacman->setPosition(xOnlyPos);
            }
        } else {
            // Try Y-only movement
            Position yOnlyPos = Position(currentPos.x, currentPos.y + movement.y);
            BoundingBox yBox(
                yOnlyPos.x - pacman->getCollisionRadius(),
                yOnlyPos.y - pacman->getCollisionRadius(),
                pacman->getCollisionRadius() * 2.0f,
                pacman->getCollisionRadius() * 2.0f
            );

            bool yCollision = false;
            for (const auto& wall : walls) {
                if (yBox.intersects(wall->getBoundingBox())) {
                    yCollision = true;
                    break;
                }
            }

            if (!yCollision) {
                pacman->setPosition(yOnlyPos);
            }
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
            activateFearMode();
        }
    }

    // Ghost collisions
    for (auto& ghost : ghosts) {
        if (ghost->getState() == GhostState::IN_SPAWN) continue;

        if (pacman->intersects(*ghost)) {
            if (ghost->isScared()) {
                ghost->die();
            } else {
                pacman->loseLife();

                if (pacman->isAlive()) {
                    deathAnimationPlaying = true;
                    deathAnimationTimer = 0.0f;
                }

                break;
            }
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
    // Fear duration
    fearModeDuration = std::max(2.0f, 5.0f - (currentLevel - 1) * 0.5f);

    // Ghost speed: +10% per level
    float speedMultiplier = 1.0f + (currentLevel - 1) * 0.1f;
    float newSpeed = 0.3f * speedMultiplier;

    for (auto& ghost : ghosts) {
        ghost->setNormalSpeed(newSpeed);
        if (ghost->getState() != GhostState::SCARED) {
            ghost->setSpeed(newSpeed);
        }
    }

    std::cout << "Level " << currentLevel
              << " - Ghost speed: " << newSpeed
              << ", Fear: " << fearModeDuration << "s" << std::endl;
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
    this->attach(&score);
    applyDifficultyScaling();
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

    if (camera) {
        camera->setMapDimensions(mapRows, mapCols);
    }

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
            case '.':
                coins.push_back(factory->createCoin(worldPos));
                coins.back()->attach(&score);
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
                fruits.back()->attach(&score);
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
        ghost->attach(&score);  // ✅ TOEVOEGEN
        ghosts.push_back(std::move(ghost));
    }

    if (pinkSpawned) {
        auto ghost = factory->createGhost(pinkPos, GhostColor::PINK);
        ghost->setWorld(this);
        ghost->attach(&score);  // ✅ TOEVOEGEN
        ghosts.push_back(std::move(ghost));
    }

    if (cyanSpawned) {
        auto ghost = factory->createGhost(cyanPos, GhostColor::BLUE);
        ghost->setWorld(this);
        ghost->attach(&score);  // ✅ TOEVOEGEN
        ghosts.push_back(std::move(ghost));
    }

    if (orangeSpawned) {
        auto ghost = factory->createGhost(orangePos, GhostColor::ORANGE);
        ghost->setWorld(this);
        ghost->attach(&score);  // ✅ TOEVOEGEN
        ghosts.push_back(std::move(ghost));
    }

    float tileSize = 2.0f / mapRows;
    std::cout << "Tile size: " << tileSize << std::endl;

    // ✅ UPDATE COLLISION RADII
    if (pacman) {
        pacman->setCollisionRadius(tileSize * 0.45f);
    }

    for (auto& ghost : ghosts) {
        ghost->setCollisionRadius(tileSize * 0.45f);
    }

    for (auto& wall : walls) {
        wall->setCollisionRadius(tileSize * 0.50f);
    }

    for (auto& coin : coins) {
        coin->setCollisionRadius(tileSize * 0.15f);
    }

    for (auto& fruit : fruits) {
        fruit->setCollisionRadius(tileSize * 0.30f);
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

void World::activateFearMode() {
    std::cout << "FEAR MODE ACTIVATED! Duration: " << fearModeDuration << "s" << std::endl;

    for (auto& ghost : ghosts) {
        ghost->enterScaredMode(fearModeDuration);
    }
}

std::string World::getMapFileForLevel(int level) const {
    switch(level) {
    case 1:  return "../resources/maps/map.txt";
    case 2:  return "../resources/maps/map_big.txt";
    case 3:  return "../resources/maps/map_big2.txt";
    case 4:  return "../resources/maps/map_big3.txt";
    default: return "../resources/maps/map_big.txt";  // blijf bij normal level
    }
}


} // namespace pacman