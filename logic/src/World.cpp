#include "logic/World.h"
#include "logic/utils/Stopwatch.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

namespace pacman {

World::World(AbstractFactory* factory)
    : factory(factory) {
}

void World::update(float deltaTime) {
    // Update score timer
    score.updateComboTimer(deltaTime);

    // Update PacMan with per-axis collision checking
    if (pacman) {
        updatePacManWithCollisions(deltaTime);
    }

    // ✅ UPDATE: Ghosts with AI and collision detection
    for (auto& ghost : ghosts) {
        // Update internal state (spawn timer, fear mode timer, movement)
        ghost->update(deltaTime);

        // Update AI to choose direction (only if not spawning)
        if (ghost->getMode() != GhostMode::SPAWNING && pacman) {
            ghost->updateAI(*pacman, deltaTime);
        }

        // Apply collision detection with walls (only if actively moving)
        if (ghost->getMode() != GhostMode::SPAWNING) {
            updateGhostWithCollisions(ghost.get(), deltaTime);
        }
    }

    // Handle entity collisions (not wall collisions)
    handleCollisions();

    // Check level complete
    if (isLevelComplete()) {
        Event event;
        event.type = EventType::LEVEL_CLEARED;
        event.value = 500 * currentLevel;
        score.onNotify(event);
    }
}

bool World::canMoveInDirection(const Position& pos, Direction dir, float radius) const {
    // Calculate where PacMan would be if moving in this direction
    Position dirVector = getDirectionVector(dir);

    // Test a small step ahead to see if this direction is viable
    const float TEST_DISTANCE = 0.02f;  // Small test distance
    Position testPos = Position(
        pos.x + dirVector.x * TEST_DISTANCE,
        pos.y + dirVector.y * TEST_DISTANCE
    );

    // Create a temporary bounding box at the test position
    BoundingBox testBox(
        testPos.x - radius,
        testPos.y - radius,
        radius * 2.0f,
        radius * 2.0f
    );

    // Check if this test position would collide with any wall
    for (const auto& wall : walls) {
        if (testBox.intersects(wall->getBoundingBox())) {
            return false;  // Can't move in this direction
        }
    }

    return true;  // Direction is viable
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
            // Activate fear mode for all ghosts
            for (auto& ghost : ghosts) {
                ghost->enterFearMode(10.0f);
            }
        }
    }

    // Check ghost collisions
    // Only check if enough time has passed since last death (invulnerability period)
    if (timeSinceLastDeath >= DEATH_COOLDOWN) {
        for (auto& ghost : ghosts) {
            if (pacman->intersects(*ghost)) {
                if (ghost->isFeared()) {
                    // Eat the ghost
                    Event event;
                    event.type = EventType::GHOST_EATEN;
                    event.value = 200;
                    score.onNotify(event);
                    ghost->respawn(Position(0, 0));  // Respawn at center
                } else if (ghost->getMode() == GhostMode::CHASING) {
                    // PacMan dies
                    std::cout << "PacMan hit by ghost! Lives before: " << pacman->getLives() << std::endl;

                    pacman->loseLife();

                    std::cout << "Lives after: " << pacman->getLives() << std::endl;

                    // Reset death timer for invulnerability period
                    timeSinceLastDeath = 0.0f;

                    std::cout << "Starting " << DEATH_COOLDOWN << "s invulnerability period" << std::endl;

                    // Reset positions
                    reset();

                    // Break to prevent multiple deaths in one frame
                    break;
                }
            }
        }
    } else {
        // Still in invulnerability period, increment timer
        timeSinceLastDeath += Stopwatch::getInstance().getDeltaTime();
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

    // Reset ghosts to their center position
    for (auto& ghost : ghosts) {
        ghost->reset(ghostCenterPosition);
    }
}

void World::applyDifficultyScaling() {
    // Increase ghost speed
    for (auto& ghost : ghosts) {
        ghost->setSpeed(ghost->getSpeed() * 1.1f);
    }
}

void World::updateGhostWithCollisions(Ghost* ghost, float deltaTime) {
    Direction currentDir = ghost->getCurrentDirection();

    // If ghost isn't moving, no collision detection needed
    if (currentDir == Direction::NONE) {
        return;
    }

    Position currentPos = ghost->getPosition();
    Position dirVector = getDirectionVector(currentDir);
    float speed = ghost->getSpeed();

    // Calculate full movement for this frame
    Position movement = dirVector * speed * deltaTime;

    // Per-axis collision detection with AABB (same approach as PacMan)
    if (currentDir == Direction::LEFT || currentDir == Direction::RIGHT) {
        // ===== HORIZONTAL MOVEMENT =====
        Position testPos = Position(currentPos.x + movement.x, currentPos.y);

        // Temporarily move ghost to test position
        Position originalPos = ghost->getPosition();
        ghost->setPosition(testPos);

        // Check collision with all walls
        bool collisionX = false;
        for (const auto& wall : walls) {
            if (ghost->intersects(*wall)) {
                collisionX = true;
                break;
            }
        }

        if (collisionX) {
            // Restore original position - can't move in X direction
            ghost->setPosition(originalPos);
        }

    } else if (currentDir == Direction::UP || currentDir == Direction::DOWN) {
        // ===== VERTICAL MOVEMENT =====
        Position testPos = Position(currentPos.x, currentPos.y + movement.y);

        Position originalPos = ghost->getPosition();
        ghost->setPosition(testPos);

        bool collisionY = false;
        for (const auto& wall : walls) {
            if (ghost->intersects(*wall)) {
                collisionY = true;
                break;
            }
        }

        if (collisionY) {
            // Restore original position - can't move in Y direction
            ghost->setPosition(originalPos);
        }
    }
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

    // Track ghost spawn positions by color
    Position redGhostPos(0, 0);
    Position pinkGhostPos(0, 0);
    Position cyanGhostPos(0, 0);
    Position orangeGhostPos(0, 0);
    bool redSpawned = false;
    bool pinkSpawned = false;
    bool cyanSpawned = false;
    bool orangeSpawned = false;

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

            case 'r':  // Red ghost spawn
            case 'R':
                redGhostPos = worldPos;
                redSpawned = true;
                std::cout << "Found Red ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'o':  // Orange ghost spawn
            case 'O':
                orangeGhostPos = worldPos;
                orangeSpawned = true;
                std::cout << "Found Orange ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'g':  // Green/Cyan ghost spawn (we'll use cyan)
            case 'G':
                cyanGhostPos = worldPos;
                cyanSpawned = true;
                std::cout << "Found Cyan ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'i':  // pInk ghost spawn (using 'i' since 'p' is pacman)
            case 'I':
                pinkGhostPos = worldPos;
                pinkSpawned = true;
                std::cout << "Found Pink ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'c':  // Coin/Fruit (power pellet)
            case 'C':
                fruits.push_back(factory->createFruit(worldPos));
                std::cout << "Found Fruit spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            default:
                // Unknown character, ignore
                    break;
            }
        }
    }

    std::cout << "Created " << wallCount << " walls and " << coinCount << " coins" << std::endl;

    // ✅ ONLY create PacMan if specified in map
    if (pacmanSpawned) {
        pacmanSpawnPosition = pacmanSpawnPos;
        pacman = factory->createPacMan(pacmanSpawnPos);
        pacman->attach(&score);
        std::cout << "Created PacMan at (" << pacmanSpawnPos.x << ", " << pacmanSpawnPos.y << ")" << std::endl;
    } else {
        std::cout << "WARNING: No PacMan ('p' or 'P') found in map - no PacMan spawned!" << std::endl;
        // Don't create PacMan at all!
    }

    // ✅ Calculate ghost center (for respawn after death)
    if (redSpawned || pinkSpawned || cyanSpawned || orangeSpawned) {
        Position sumPos(0, 0);
        int count = 0;
        if (redSpawned) { sumPos = sumPos + redGhostPos; count++; }
        if (pinkSpawned) { sumPos = sumPos + pinkGhostPos; count++; }
        if (cyanSpawned) { sumPos = sumPos + cyanGhostPos; count++; }
        if (orangeSpawned) { sumPos = sumPos + orangeGhostPos; count++; }
        ghostCenterPosition = Position(sumPos.x / count, sumPos.y / count);
    } else {
        ghostCenterPosition = Position(0, 0);
    }

    // ✅ ONLY create ghosts that exist in map
    int ghostsCreated = 0;

    if (redSpawned) {
        auto ghost1 = factory->createGhost(redGhostPos, GhostType::RANDOM);
        ghost1->setSpawnDelay(0.0f);
        ghosts.push_back(std::move(ghost1));
        ghostsCreated++;
    }

    if (pinkSpawned) {
        auto ghost2 = factory->createGhost(pinkGhostPos, GhostType::CHASER);
        ghost2->setSpawnDelay(0.0f);
        ghosts.push_back(std::move(ghost2));
        ghostsCreated++;
    }

    if (cyanSpawned) {
        auto ghost3 = factory->createGhost(cyanGhostPos, GhostType::PREDICTOR);
        ghost3->setSpawnDelay(5.0f);
        ghosts.push_back(std::move(ghost3));
        ghostsCreated++;
    }

    if (orangeSpawned) {
        auto ghost4 = factory->createGhost(orangeGhostPos, GhostType::CHASER);
        ghost4->setSpawnDelay(10.0f);
        ghosts.push_back(std::move(ghost4));
        ghostsCreated++;
    }

    std::cout << "Created " << ghostsCreated << " ghosts" << std::endl;
    std::cout << "Created " << fruits.size() << " fruits from map" << std::endl;
    std::cout << "Map loading complete!" << std::endl;
}


} // namespace pacman