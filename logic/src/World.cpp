#include "logic/World.h"
#include "logic/utils/Stopwatch.h"
#include <fstream>
#include <sstream>
#include <iostream>

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

    // Update ghosts
    for (auto& ghost : ghosts) {
        ghost->update(deltaTime);
        // TODO: Add ghost collision with walls later
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

void World::updatePacManWithCollisions(float deltaTime) {
    Direction dir = pacman->getDirection();
    if (dir == Direction::NONE) {
        return;
    }

    Position currentPos = pacman->getPosition();
    Position dirVector = getDirectionVector(dir);
    float speed = pacman->getSpeed();

    // Calculate full movement
    Position movement = dirVector * speed * deltaTime;

    // ✅ CRITICAL: Small epsilon for float precision
    const float EPSILON = 0.001f;

    // ✅ Per-axis collision detection with AABB
    if (dir == Direction::LEFT || dir == Direction::RIGHT) {
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
        // else: keep the new position (movement successful)

    } else if (dir == Direction::UP || dir == Direction::DOWN) {
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
        // Still in invulnerability period
        // Optionally log this for debugging
        // std::cout << "Invulnerable (" << (DEATH_COOLDOWN - timeSinceLastDeath) << "s remaining)" << std::endl;
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

    // Create PacMan
    if (!pacmanSpawned) {
        // Default spawn near bottom center if not specified in map
        pacmanSpawnPos = Position(0, 0.73f);
        std::cout << "Using default PacMan spawn position" << std::endl;
    }

    // Save spawn position for later resets
    pacmanSpawnPosition = pacmanSpawnPos;

    pacman = factory->createPacMan(pacmanSpawnPos);
    pacman->attach(&score);
    std::cout << "Created PacMan at (" << pacmanSpawnPos.x << ", " << pacmanSpawnPos.y << ")" << std::endl;

    // Create ghosts at their spawn positions
    // If no spawn position specified, use default center position
    Position defaultGhostPos(0, 0);

    // Save ghost center for resets
    ghostCenterPosition = defaultGhostPos;

    // Ghost 1: Red (RANDOM type)
    Position ghost1Pos = redSpawned ? redGhostPos : defaultGhostPos;
    auto ghost1 = factory->createGhost(ghost1Pos, GhostType::RANDOM);
    ghost1->setSpawnDelay(0.0f);
    ghosts.push_back(std::move(ghost1));

    // Ghost 2: Pink (CHASER type)
    Position ghost2Pos = pinkSpawned ? pinkGhostPos : defaultGhostPos + Position(0.1f, 0);
    auto ghost2 = factory->createGhost(ghost2Pos, GhostType::CHASER);
    ghost2->setSpawnDelay(0.0f);
    ghosts.push_back(std::move(ghost2));

    // Ghost 3: Cyan (PREDICTOR type)
    Position ghost3Pos = cyanSpawned ? cyanGhostPos : defaultGhostPos + Position(-0.1f, 0);
    auto ghost3 = factory->createGhost(ghost3Pos, GhostType::PREDICTOR);
    ghost3->setSpawnDelay(5.0f);
    ghosts.push_back(std::move(ghost3));

    // Ghost 4: Orange (CHASER type)
    Position ghost4Pos = orangeSpawned ? orangeGhostPos : defaultGhostPos + Position(0, 0.1f);
    auto ghost4 = factory->createGhost(ghost4Pos, GhostType::CHASER);
    ghost4->setSpawnDelay(10.0f);
    ghosts.push_back(std::move(ghost4));

    std::cout << "Created 4 ghosts" << std::endl;

    // Add default fruits if none specified in map
    if (fruits.empty()) {
        fruits.push_back(factory->createFruit(Position(-0.8f, -0.8f)));  // Top-left
        fruits.push_back(factory->createFruit(Position(0.8f, 0.8f)));    // Bottom-right
        std::cout << "Created 2 default fruits" << std::endl;
    }

    std::cout << "Map loading complete!" << std::endl;
}

} // namespace pacman