#include "logic/entities/Ghost.h"
#include "logic/entities/PacMan.h"
#include "logic/utils/Random.h"
#include "logic/utils/Stopwatch.h"
#include <cmath>
#include <limits>

namespace pacman {

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
    // Only update AI if not spawning
    if (mode == GhostMode::SPAWNING) {
        return;
    }

    // Choose new direction at intersections
    if (isAtIntersection()) {
        Direction newDirection = chooseDirection(pacman);
        if (newDirection != Direction::NONE && newDirection != currentDirection) {
            currentDirection = newDirection;
        }
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
    // Chase PacMan by minimizing Manhattan distance
    auto viable = getViableDirections();
    if (viable.empty()) return currentDirection;

    Position pacmanPos = pacman.getPosition();
    Direction bestDirection = currentDirection;
    float bestDistance = std::numeric_limits<float>::max();

    // Try each viable direction and pick the one that gets closest to PacMan
    for (Direction dir : viable) {
        // Don't go backwards unless it's the only option
        if (isOppositeDirection(dir, currentDirection) && viable.size() > 1) {
            continue;
        }

        // Calculate where we'd be if we moved in this direction
        Position dirVector = getDirectionVector(dir);
        Position testPos = position + dirVector * 0.1f;  // Small step ahead

        // Calculate Manhattan distance to PacMan
        float distance = testPos.manhattanDistance(pacmanPos);

        if (distance < bestDistance) {
            bestDistance = distance;
            bestDirection = dir;
        }
    }

    return bestDirection;
}

Direction Ghost::choosePredictorDirection(const PacMan& pacman) {
    // Move towards where PacMan is facing (predict future position)
    auto viable = getViableDirections();
    if (viable.empty()) return currentDirection;

    Position pacmanPos = pacman.getPosition();
    Direction pacmanDir = pacman.getDirection();

    // Predict where PacMan will be (2 tiles ahead)
    Position dirVector = getDirectionVector(pacmanDir);
    Position predictedPos = pacmanPos + dirVector * 0.4f;

    Direction bestDirection = currentDirection;
    float bestDistance = std::numeric_limits<float>::max();

    // Try each viable direction and pick the one that gets closest to predicted position
    for (Direction dir : viable) {
        // Don't go backwards unless it's the only option
        if (isOppositeDirection(dir, currentDirection) && viable.size() > 1) {
            continue;
        }

        Position testDirVector = getDirectionVector(dir);
        Position testPos = position + testDirVector * 0.1f;

        float distance = testPos.manhattanDistance(predictedPos);

        if (distance < bestDistance) {
            bestDistance = distance;
            bestDirection = dir;
        }
    }

    return bestDirection;
}

Direction Ghost::chooseFearDirection(const PacMan& pacman) {
    // Run away: maximize distance instead of minimize
    auto viable = getViableDirections();
    if (viable.empty()) return currentDirection;

    Position pacmanPos = pacman.getPosition();
    Direction bestDirection = currentDirection;
    float bestDistance = 0.0f;  // Start with minimum

    // Try each viable direction and pick the one that gets furthest from PacMan
    for (Direction dir : viable) {
        // Don't go backwards unless it's the only option
        if (isOppositeDirection(dir, currentDirection) && viable.size() > 1) {
            continue;
        }

        Position dirVector = getDirectionVector(dir);
        Position testPos = position + dirVector * 0.1f;

        float distance = testPos.manhattanDistance(pacmanPos);

        if (distance > bestDistance) {
            bestDistance = distance;
            bestDirection = dir;
        }
    }

    return bestDirection;
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
    // For simplified AI: always allow direction changes
    // In more advanced implementation, check grid alignment
    return true;
}

std::vector<Direction> Ghost::getViableDirections() const {
    // Return all cardinal directions
    // World will filter out directions blocked by walls
    std::vector<Direction> directions;
    directions.push_back(Direction::UP);
    directions.push_back(Direction::DOWN);
    directions.push_back(Direction::LEFT);
    directions.push_back(Direction::RIGHT);
    return directions;
}

bool Ghost::isOppositeDirection(Direction dir1, Direction dir2) const {
    return (dir1 == Direction::UP && dir2 == Direction::DOWN) ||
           (dir1 == Direction::DOWN && dir2 == Direction::UP) ||
           (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) ||
           (dir1 == Direction::RIGHT && dir2 == Direction::LEFT);
}

} // namespace pacman