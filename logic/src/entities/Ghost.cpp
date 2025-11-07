#include "logic/entities/Ghost.h"
#include "logic/entities/PacMan.h"
#include "logic/utils/Random.h"
#include "logic/utils/Stopwatch.h"

namespace pacman {class PacMan;

Ghost::Ghost(const Position& pos, GhostType type)
    : EntityModel(pos, 0.3f), type(type), startPosition(pos) {
}

void Ghost::update(float deltaTime) {
    // Handle spawning delay
    if (mode == GhostMode::SPAWNING) {
        spawnTimer -= deltaTime;
        if (spawnTimer <= 0) {
            mode = GhostMode::CHASING;
        }
        return;
    }
    
    // Handle fear mode timer
    if (mode == GhostMode::FEAR) {
        fearTimer -= deltaTime;
        if (fearTimer <= 0) {
            mode = GhostMode::CHASING;
            speed /= 0.5f;  // Restore original speed
        }
    }
    
    // Move in current direction
    Position dirVector = getDirectionVector(currentDirection);
    position = position + dirVector * speed * deltaTime;
}

void Ghost::updateAI(const PacMan& pacman, float deltaTime) {
    // Choose new direction at intersections
    if (isAtIntersection()) {
        currentDirection = chooseDirection(pacman);
    }
}

Direction Ghost::chooseDirection(const PacMan& pacman) {
    if (mode == GhostMode::FEAR) {
        return chooseFearDirection(pacman);
    }
    
    switch (type) {
        case GhostType::RANDOM:
            return chooseRandomDirection();
        case GhostType::CHASER:
            return chooseChasingDirection(pacman);
        case GhostType::PREDICTOR:
            return choosePredictorDirection(pacman);
        default:
            return currentDirection;
    }
}

Direction Ghost::chooseRandomDirection() {
    auto viable = getViableDirections();
    if (viable.empty()) return currentDirection;
    
    if (Random::getInstance().getBool(0.5f)) {
        // 50% chance to lock to random direction
        int idx = Random::getInstance().getInt(0, viable.size() - 1);
        lockedDirection = viable[idx];
    }
    return lockedDirection;
}

Direction Ghost::chooseChasingDirection(const PacMan& pacman) {
    // TODO: Implement Manhattan distance minimization
    return currentDirection;
}

Direction Ghost::choosePredictorDirection(const PacMan& pacman) {
    // TODO: Move towards where PacMan is facing
    return currentDirection;
}

Direction Ghost::chooseFearDirection(const PacMan& pacman) {
    // TODO: Maximize distance instead of minimize
    return currentDirection;
}

void Ghost::enterFearMode(float duration) {
    mode = GhostMode::FEAR;
    fearDuration = duration;
    fearTimer = duration;
    speed *= 0.5f;  // Slow down
    
    // Reverse direction
    switch (currentDirection) {
        case Direction::UP: currentDirection = Direction::DOWN; break;
        case Direction::DOWN: currentDirection = Direction::UP; break;
        case Direction::LEFT: currentDirection = Direction::RIGHT; break;
        case Direction::RIGHT: currentDirection = Direction::LEFT; break;
        default: break;
    }
}

void Ghost::setMode(GhostMode newMode) {
    mode = newMode;
}

void Ghost::reset(const Position& centerPos) {
    position = centerPos;
    startPosition = centerPos;
    mode = GhostMode::SPAWNING;
    currentDirection = Direction::RIGHT;
}

void Ghost::respawn(const Position& centerPos) {
    position = centerPos;
    mode = GhostMode::CHASING;
}

bool Ghost::isAtIntersection() const {
    // TODO: Check if at grid intersection
    return false;
}

std::vector<Direction> Ghost::getViableDirections() const {
    // TODO: Check which directions don't hit walls
    return {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
}

} // namespace pacman