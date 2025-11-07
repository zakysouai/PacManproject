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

    // Update PacMan (only if it exists)
    if (pacman) {
        pacman->update(deltaTime);
    }

    // Update ghosts
    for (auto& ghost : ghosts) {
        ghost->update(deltaTime);
    }

    // Handle collisions
    handleCollisions();

    // Check level complete
    if (isLevelComplete()) {
        Event event;
        event.type = EventType::LEVEL_CLEARED;
        event.value = 500 * currentLevel;  // Bonus points
        score.onNotify(event);
    }
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
                pacman->loseLife();
                // Reset positions
                reset();
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
    // Reset PacMan and ghosts to start positions
    if (pacman) {
        pacman->reset(Position(0, 0.5f));
    }
    for (auto& ghost : ghosts) {
        ghost->reset(Position(0, 0));
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

    // Clear pacman as well for now
    pacman.reset();

    int wallCount = 0;

    // Parse the map and create ONLY walls for now
    // This allows you to verify the map is loading correctly
    for (int row = 0; row < totalRows; ++row) {
        for (int col = 0; col < totalCols; ++col) {
            if (col >= static_cast<int>(mapData[row].length())) continue;

            char tile = mapData[row][col];
            Position worldPos = gridToWorld(row, col, totalRows, totalCols);

            // Only create walls - ignore all other characters for now
            if (tile == 'x' || tile == 'X') {
                walls.push_back(factory->createWall(worldPos));
                wallCount++;
            }
            // You can optionally add small markers for empty spaces to verify positioning:
            // else if (tile == ' ') {
            //     coins.push_back(factory->createCoin(worldPos));
            // }
        }
    }

    std::cout << "Created " << wallCount << " walls" << std::endl;
    std::cout << "Map loading complete! (Only walls - PacMan, ghosts, coins, fruits not created yet)" << std::endl;
}

} // namespace pacman