#include "logic/entities/Ghost.h"
#include "logic/entities/PacMan.h"
#include "logic/World.h"
#include "logic/utils/Random.h"
#include "logic/utils/Stopwatch.h"
#include <cmath>
#include <iostream>
#include <limits>

namespace pacman {

Ghost::Ghost(const Position& pos, GhostType type)
    : EntityModel(pos, 0.3f), type(type), startPosition(pos) {

    // ✅ ALLE ghosts gaan UP uit spawn
    currentDirection = Direction::UP;
    lockedDirection = Direction::UP;
}

void Ghost::update(float deltaTime) {
    // Handle spawning delay
    if (mode == GhostMode::SPAWNING && spawnTimer > 0) {
        spawnTimer -= deltaTime;
        return;
    }

    // Check if left spawn
    if (mode == GhostMode::SPAWNING && spawnTimer <= 0) {
        if (hasLeftSpawn()) {
            mode = GhostMode::CHASING;
            std::cout << "Ghost type " << static_cast<int>(type)
                      << " left spawn!" << std::endl;
        }
    }

    // Handle fear mode
    if (mode == GhostMode::FEAR) {
        fearTimer -= deltaTime;
        if (fearTimer <= 0) {
            mode = GhostMode::CHASING;
            speed /= 0.5f;
        }
    }
}

void Ghost::updateAI(const PacMan& pacman, float deltaTime) {
    // ✅ SPAWNING ghosts now use AI to find the exit
    if (mode == GhostMode::SPAWNING) {
        if (isAtIntersection()) {
            Direction newDirection = chooseSpawnExitDirection();
            if (newDirection != Direction::NONE && newDirection != currentDirection) {
                currentDirection = newDirection;
            }
        }
        return;
    }

    // Rest of existing AI logic for CHASING/FEAR modes
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
    // ✅ UPDATED: Now gets viable directions (filtered by walls)
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
    // ✅ UPDATED: Now gets viable directions (filtered by walls)
    auto viable = getViableDirections();
    if (viable.empty()) return currentDirection;

    Position pacmanPos = pacman.getPosition();
    Direction bestDirection = currentDirection;
    float bestDistance = std::numeric_limits<float>::max();

    // Try each viable direction and pick the one that gets closest to PacMan
    for (Direction dir : viable) {
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
    // ✅ UPDATED: Now gets viable directions (filtered by walls)
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
    // ✅ UPDATED: Now gets viable directions (filtered by walls)
    auto viable = getViableDirections();
    if (viable.empty()) return currentDirection;

    Position pacmanPos = pacman.getPosition();
    Direction bestDirection = currentDirection;
    float bestDistance = 0.0f;  // Start with minimum

    // Try each viable direction and pick the one that gets furthest from PacMan
    for (Direction dir : viable) {
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

Direction Ghost::chooseSpawnExitDirection() {
    if (!world) return currentDirection;

    auto viable = getViableDirections();
    if (viable.empty()) return currentDirection;

    Position exitPos = world->getGhostExitPosition();

    // If we can see the exit in a straight line, go directly toward it
    for (Direction dir : viable) {
        if (canSeeExitInDirection(dir, exitPos)) {
            return dir;
        }
    }

    // Otherwise, use Manhattan distance to choose the best direction
    Direction bestDirection = currentDirection;
    float bestDistance = std::numeric_limits<float>::max();

    for (Direction dir : viable) {
        Position testPos = simulateMovement(dir);
        float distance = testPos.manhattanDistance(exitPos);

        if (distance < bestDistance) {
            bestDistance = distance;
            bestDirection = dir;
        }
    }

    return bestDirection;
}

bool Ghost::canSeeExitInDirection(Direction dir, const Position& exitPos) const {
    // Check if moving in this direction would get us closer to exit without hitting walls
    Position testPos = simulateMovement(dir);

    // Calculate if we're moving toward the exit
    float currentDist = position.manhattanDistance(exitPos);
    float newDist = testPos.manhattanDistance(exitPos);

    return newDist < currentDist && world->canMoveInDirection(position, dir, getCollisionRadius());
}

Position Ghost::simulateMovement(Direction dir) const {
    Position dirVector = getDirectionVector(dir);
    return position + dirVector * 0.3f; // Simulate moving a short distance
}

Direction Ghost::getDirectionToTarget(const Position& target) const {
    float dx = target.x - position.x;
    float dy = target.y - position.y;

    // Prefer horizontal movement if more significant
    if (std::abs(dx) > std::abs(dy)) {
        return (dx > 0) ? Direction::RIGHT : Direction::LEFT;
    } else {
        return (dy > 0) ? Direction::UP : Direction::DOWN;
    }
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

    // ✅ UPDATED: Set initial direction based on ghost type to spread them out
    switch (type) {
        case GhostType::RANDOM:
            currentDirection = Direction::UP;
            break;
        case GhostType::CHASER:
            currentDirection = Direction::RIGHT;
            break;
        case GhostType::PREDICTOR:
            currentDirection = Direction::DOWN;
            break;
        default:
            currentDirection = Direction::LEFT;
            break;
    }

    lockedDirection = currentDirection;
}

void Ghost::respawn(const Position& centerPos) {
    position = centerPos;
    mode = GhostMode::CHASING;
}

bool Ghost::isAtIntersection() const {
    if (!world) {
        return false;  // ← Verander naar false (geen world = geen intersection check)
    }

    if (currentDirection == Direction::NONE) {
        return true;
    }

    float radius = getCollisionRadius();

    // ✅ Check of huidige richting geblokkeerd is (dead end - MOET draaien)
    if (!world->canMoveInDirection(position, currentDirection, radius)) {
        return true;
    }

    // ✅ Check perpendicular richtingen
    std::vector<Direction> perpendicular;

    if (currentDirection == Direction::UP || currentDirection == Direction::DOWN) {
        perpendicular = {Direction::LEFT, Direction::RIGHT};
    } else {
        perpendicular = {Direction::UP, Direction::DOWN};
    }

    // ✅ Tel aantal viable perpendicular richtingen
    int viableCount = 0;
    for (Direction dir : perpendicular) {
        if (world->canMoveInDirection(position, dir, radius)) {
            viableCount++;
        }
    }

    // ✅ Alleen intersection als er minimaal 1 perpendicular optie is
    return viableCount >= 1;
}

// ✅ UPDATED: Now filters out blocked directions using world pointer
std::vector<Direction> Ghost::getViableDirections() const {
    std::vector<Direction> all = {
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT
    };

    // If we don't have world access, return all directions (fallback)
    if (!world) {
        return all;
    }

    // Filter out directions that are blocked by walls or opposite to current direction
    std::vector<Direction> viable;
    float radius = getCollisionRadius();

    for (Direction dir : all) {
        // Don't allow 180° turns yet (we'll add them back if no other option)
        if (isOppositeDirection(dir, currentDirection)) {
            continue;
        }

        // Check if this direction is viable (not blocked by wall)
        if (world->canMoveInDirection(position, dir, radius)) {
            viable.push_back(dir);
        }
    }

    // ✅ UPDATED: If no viable directions found (dead end), allow 180° turn
    if (viable.empty()) {
        for (Direction dir : all) {
            // Now allow opposite direction
            if (world->canMoveInDirection(position, dir, radius)) {
                viable.push_back(dir);
            }
        }
    }

    // Last resort: if still no viable directions, return current direction
    // (this means ghost is completely stuck, which shouldn't happen)
    if (viable.empty()) {
        viable.push_back(currentDirection);
    }

    return viable;
}

bool Ghost::isOppositeDirection(Direction dir1, Direction dir2) const {
    return (dir1 == Direction::UP && dir2 == Direction::DOWN) ||
           (dir1 == Direction::DOWN && dir2 == Direction::UP) ||
           (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) ||
           (dir1 == Direction::RIGHT && dir2 == Direction::LEFT);
}

bool Ghost::hasLeftSpawn() const {
    if (!world) return true;

    Position exit = world->getGhostExitPosition();
    float distanceToExit = position.distance(exit);

    // ✅ Close enough to exit = successfully exited spawn
    return distanceToExit < 0.3f;
}





} // namespace pacman