#include "logic/World.h"
#include "logic/utils/Stopwatch.h"
#include <fstream>
#include <sstream>

namespace pacman {

World::World(AbstractFactory* factory)
    : factory(factory) {
}

void World::update(float deltaTime) {
    // Update score timer
    score.updateComboTimer(deltaTime);
    
    // Update PacMan
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
    // Convert grid coordinates to normalized world coordinates [-1, 1]
    float x = -1.0f + (2.0f * col / totalCols);
    float y = -1.0f + (2.0f * row / totalRows);
    return Position(x, y);
}

void World::loadLevel(const std::string& mapFile) {
    parseMap(mapFile);
}

void World::parseMap(const std::string& mapFile) {
    // TODO: Implement map parsing
    // Read map.txt file and spawn entities
}

} // namespace pacman