#include "logic/World.h"

#include "logic/utils/Random.h"
#include "logic/utils/Stopwatch.h"
#include "logic/entities/Ghost.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

namespace pacman {

World::World(AbstractFactory& factory, std::shared_ptr<Camera> camera)  // ✅ & niet *
    : factory(factory), camera(camera) {
}

void World::update(float deltaTime) {
    if (deathAnimationPlaying) {
        deathAnimationTimer += deltaTime;

        if (pacman) {
            pacman->update(deltaTime);
        }

        if (deathAnimationTimer >= DEATH_ANIMATION_DURATION) {
            deathAnimationPlaying = false;
            deathAnimationTimer = 0.0f;

            pacman->reset(pacmanSpawnPosition);
            for (auto& g : ghosts) {
                g->reset();
            }
        }
        return;
    }

    score.updateComboTimer(deltaTime);

    if (pacman) {
        updatePacManWithCollisions(deltaTime);
        checkWraparound(pacman.get());
        pacman->update(deltaTime);
    }

    for (auto& ghost : ghosts) {
        ghost->update(deltaTime);
        checkWraparound(ghost.get());
    }

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

    float mapAspectRatio = static_cast<float>(mapCols) / static_cast<float>(mapRows);
    float worldWidth = mapAspectRatio;

    const float THRESHOLD = 0.05f;

    Position pos = entity->getPosition();

    if (pos.x > worldWidth - THRESHOLD) {
        pos.x = -worldWidth + THRESHOLD;
        entity->setPosition(pos);
    }
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

    for (const auto& wall : walls) {
        if (testBox.intersects(wall->getBoundingBox())) {
            return true;
        }
    }

    if (hasDoor) {
        auto testGrid = worldToGrid(pos);
        if (testGrid.row == doorGridPos.row && testGrid.col == doorGridPos.col) {
            if (!ghost) return true;
            return ghost->hasPassedThroughDoor();
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

    for (const auto& wall : walls) {
        if (testBox.intersects(wall->getBoundingBox())) {
            return false;
        }
    }

    if (hasDoor) {
        auto testGrid = worldToGrid(testPos);
        if (testGrid.row == doorGridPos.row && testGrid.col == doorGridPos.col) {
            if (!ghost) return false;
            return !ghost->hasPassedThroughDoor();
        }
    }

    return true;
}

bool World::isAtIntersection(const Position& pos, Direction currentDir, float radius) const {
    if (currentDir == Direction::NONE) {
        return true;
    }

    std::vector<Direction> perpendicular;
    if (currentDir == Direction::UP || currentDir == Direction::DOWN) {
        perpendicular = {Direction::LEFT, Direction::RIGHT};
    } else {
        perpendicular = {Direction::UP, Direction::DOWN};
    }

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

    Position dirVector = getDirectionVector(currentDir);
    float speed = pacman->getSpeed();
    Position movement = dirVector * speed * deltaTime;

    Position currentPos = pacman->getPosition();
    Position testPos = currentPos + movement;

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

    float tileSize = 2.0f / mapRows;

    if (!collision) {
        pacman->setPosition(testPos);
    } else {
        // ✅ CORNER SMOOTHING: probeer kleine correctie in perpendicular richting
        const float CORNER_SMOOTH = tileSize * 0.15f;  // Kleine nudge

        if (currentDir == Direction::LEFT || currentDir == Direction::RIGHT) {
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
            // ✅ NIEUW: corner smoothing
            else {
                // Probeer kleine Y correctie omhoog
                Position correctedUp = Position(currentPos.x + movement.x, currentPos.y - CORNER_SMOOTH);
                BoundingBox testUp(
                    correctedUp.x - pacman->getCollisionRadius(),
                    correctedUp.y - pacman->getCollisionRadius(),
                    pacman->getCollisionRadius() * 2.0f,
                    pacman->getCollisionRadius() * 2.0f
                );

                bool upWorks = true;
                for (const auto& wall : walls) {
                    if (testUp.intersects(wall->getBoundingBox())) {
                        upWorks = false;
                        break;
                    }
                }

                if (upWorks) {
                    pacman->setPosition(correctedUp);
                } else {
                    // Probeer omlaag
                    Position correctedDown = Position(currentPos.x + movement.x, currentPos.y + CORNER_SMOOTH);
                    BoundingBox testDown(
                        correctedDown.x - pacman->getCollisionRadius(),
                        correctedDown.y - pacman->getCollisionRadius(),
                        pacman->getCollisionRadius() * 2.0f,
                        pacman->getCollisionRadius() * 2.0f
                    );

                    bool downWorks = true;
                    for (const auto& wall : walls) {
                        if (testDown.intersects(wall->getBoundingBox())) {
                            downWorks = false;
                            break;
                        }
                    }

                    if (downWorks) {
                        pacman->setPosition(correctedDown);
                    }
                }
            }
        } else {
            // Hetzelfde voor UP/DOWN movement
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
            // ✅ NIEUW: corner smoothing voor vertical movement
            else {
                // Probeer links
                Position correctedLeft = Position(currentPos.x - CORNER_SMOOTH, currentPos.y + movement.y);
                BoundingBox testLeft(
                    correctedLeft.x - pacman->getCollisionRadius(),
                    correctedLeft.y - pacman->getCollisionRadius(),
                    pacman->getCollisionRadius() * 2.0f,
                    pacman->getCollisionRadius() * 2.0f
                );

                bool leftWorks = true;
                for (const auto& wall : walls) {
                    if (testLeft.intersects(wall->getBoundingBox())) {
                        leftWorks = false;
                        break;
                    }
                }

                if (leftWorks) {
                    pacman->setPosition(correctedLeft);
                } else {
                    // Probeer rechts
                    Position correctedRight = Position(currentPos.x + CORNER_SMOOTH, currentPos.y + movement.y);
                    BoundingBox testRight(
                        correctedRight.x - pacman->getCollisionRadius(),
                        correctedRight.y - pacman->getCollisionRadius(),
                        pacman->getCollisionRadius() * 2.0f,
                        pacman->getCollisionRadius() * 2.0f
                    );

                    bool rightWorks = true;
                    for (const auto& wall : walls) {
                        if (testRight.intersects(wall->getBoundingBox())) {
                            rightWorks = false;
                            break;
                        }
                    }

                    if (rightWorks) {
                        pacman->setPosition(correctedRight);
                    }
                }
            }
        }
    }

}

bool World::isPositionBlocked(const Position& pos, float radius) const {
    for (const auto& wall : walls) {
        float distance = pos.distance(wall->getPosition());
        if (distance < radius + wall->getCollisionRadius()) {
            return true;
        }
    }
    return false;
}

bool World::checkWallCollision(const Position& pos, float radius) const {
    for (const auto& wall : walls) {
        float distance = pos.distance(wall->getPosition());
        float minDistance = radius + wall->getCollisionRadius();

        if (distance < minDistance) {
            return true;
        }
    }
    return false;
}

void World::handleCollisions() {
    if (!pacman) return;

    for (auto& coin : coins) {
        if (!coin->isCollected() && pacman->intersects(*coin)) {
            coin->collect();
        }
    }

    for (auto& fruit : fruits) {
        if (!fruit->isCollected() && pacman->intersects(*fruit)) {
            fruit->collect();
            activateFearMode();
        }
    }

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
    applyDifficultyScaling();
}

void World::reset() {
    if (pacman) {
        pacman->reset(pacmanSpawnPosition);
        std::cout << "Reset PacMan to spawn position ("
                  << pacmanSpawnPosition.x << ", "
                  << pacmanSpawnPosition.y << ")" << std::endl;
    }
}

void World::applyDifficultyScaling() {
    fearModeDuration = std::max(2.0f, 5.0f - (currentLevel - 1) * 0.5f);

    // ✅ NIEUWE FORMULE: 20% sneller per level
    // Level 1: 0.25 * 1.0 = 0.25
    // Level 2: 0.25 * 1.2 = 0.30
    // Level 3: 0.25 * 1.44 = 0.36
    // Level N: 0.25 * (1.2)^(N-1)

    const float BASE_GHOST_SPEED = 0.25f;  // Trager dan voorheen (was 0.3f)
    float speedMultiplier = std::pow(1.2f, currentLevel - 1);  // 20% per level
    float newSpeed = BASE_GHOST_SPEED * speedMultiplier;

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
    float mapAspectRatio = static_cast<float>(totalCols) / static_cast<float>(totalRows);

    float worldWidth = mapAspectRatio;
    float worldHeight = 1.0f;

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

    spawnEntities(mapData);
}

void World::spawnEntities(const std::vector<std::string>& mapData) {
    int totalRows = mapData.size();
    int totalCols = 0;

    for (const auto& row : mapData) {
        if (static_cast<int>(row.length()) > totalCols) {
            totalCols = row.length();
        }
    }

    mapRows = totalRows;
    mapCols = totalCols;

    if (camera) {  // ✅ Camera is now shared_ptr, direct access OK
        camera->setMapDimensions(mapRows, mapCols);
    }

    std::cout << "Map size: " << totalRows << "x" << totalCols << std::endl;

    walls.clear();
    coins.clear();
    fruits.clear();
    ghosts.clear();

    Position pacmanSpawnPos(0, 0);
    bool pacmanSpawned = false;
    int wallCount = 0;
    int coinCount = 0;

    bool redSpawned = false;
    bool pinkSpawned = false;
    bool cyanSpawned = false;
    bool orangeSpawned = false;
    Position redPos(0, 0);
    Position pinkPos(0, 0);
    Position cyanPos(0, 0);
    Position orangePos(0, 0);

    for (int row = 0; row < totalRows; ++row) {
        for (int col = 0; col < totalCols; ++col) {
            if (col >= static_cast<int>(mapData[row].length())) continue;

            char tile = mapData[row][col];
            Position worldPos = gridToWorld(row, col, totalRows, totalCols);

            switch (tile) {
            case 'x':
            case 'X':
                walls.push_back(factory.createWall(worldPos));
                wallCount++;
                break;

            case ' ':
            case '.':
                coins.push_back(factory.createCoin(worldPos));
                coins.back()->attach(&score);
                coinCount++;
                break;

            case 'p':
            case 'P':
                pacmanSpawnPos = worldPos;
                pacmanSpawned = true;
                std::cout << "Found PacMan spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'c':
            case 'C':
                fruits.push_back(factory.createFruit(worldPos));
                fruits.back()->attach(&score);
                std::cout << "Found Fruit spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'r':
            case 'R':
                redPos = worldPos;
                redSpawned = true;
                std::cout << "Found Red ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'i':
            case 'I':
                pinkPos = worldPos;
                pinkSpawned = true;
                std::cout << "Found Pink ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'b':
            case 'B':
                cyanPos = worldPos;
                cyanSpawned = true;
                std::cout << "Found Cyan ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'o':
            case 'O':
                orangePos = worldPos;
                orangeSpawned = true;
                std::cout << "Found Orange ghost spawn at grid(" << row << "," << col << ")" << std::endl;
                break;

            case 'd':
            case 'D':
                doorPosition = worldPos;
                doorGridPos = {row, col};
                hasDoor = true;
                std::cout << "Found Door at grid(" << row << "," << col << ")" << std::endl;
                break;

            default:
                break;
            }
        }
    }

    std::cout << "Created " << wallCount << " walls and " << coinCount << " coins" << std::endl;

    if (pacmanSpawned) {
        pacmanSpawnPosition = pacmanSpawnPos;
        pacman = factory.createPacMan(pacmanSpawnPos);
        pacman->attach(&score);
        std::cout << "Created PacMan at (" << pacmanSpawnPos.x << ", " << pacmanSpawnPos.y << ")" << std::endl;
    } else {
        std::cout << "WARNING: No PacMan ('p' or 'P') found in map - no PacMan spawned!" << std::endl;
    }

    if (redSpawned) {
        auto ghost = factory.createGhost(*this, redPos, GhostColor::RED);  // *this = World&
        ghost->attach(&score);
        ghosts.push_back(std::move(ghost));
    }

    if (pinkSpawned) {
        auto ghost = factory.createGhost(*this, pinkPos, GhostColor::PINK);  // *this
        ghost->attach(&score);
        ghosts.push_back(std::move(ghost));
    }

    if (cyanSpawned) {
        auto ghost = factory.createGhost(*this, cyanPos, GhostColor::BLUE);  // *this
        ghost->attach(&score);
        ghosts.push_back(std::move(ghost));
    }

    if (orangeSpawned) {
        auto ghost = factory.createGhost(*this, orangePos, GhostColor::ORANGE);  // *this
        ghost->attach(&score);
        ghosts.push_back(std::move(ghost));
    }

    float tileSize = 2.0f / mapRows;
    std::cout << "Tile size: " << tileSize << std::endl;

    if (pacman) {
        pacman->setCollisionRadius(tileSize * 0.449999f);
    }

    for (auto& ghost : ghosts) {
        ghost->setCollisionRadius(tileSize * 0.40f);
    }

    for (auto& wall : walls) {
        wall->setCollisionRadius(tileSize * 0.48f);
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
    return pos.distance(doorPosition) < 0.05f;
}

bool World::isDoorBlockingEntity(const Ghost* ghost, const Position& testPos) const {
    if (!isDoorPosition(testPos)) return false;
    return ghost && ghost->hasPassedThroughDoor();
}

bool World::isDoorBlockedFor(const Position& pos, const Ghost* ghost) const {
    if (!hasDoor) return false;
    if (!ghost) return false;

    if (!ghost->hasPassedThroughDoor()) {
        return false;
    }

    return pos.distance(doorPosition) < 0.15f;
}

World::GridPosition World::worldToGrid(const Position& worldPos) const {
    float worldWidth = static_cast<float>(mapCols) / static_cast<float>(mapRows);
    float worldHeight = 1.0f;

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
    default: return "../resources/maps/map_big.txt";
    }
}

} // namespace pacman