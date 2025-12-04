#include "logic/entities/Ghost.h"
#include "logic/World.h"
#include "logic/utils/Random.h"

#include <iostream>

namespace pacman {

Ghost::Ghost(const Position& pos, GhostColor color)
    : EntityModel(pos, 0.3f), color(color) {
}

void Ghost::update(float deltaTime) {
    move(deltaTime);
}

void Ghost::move(float deltaTime) {
    if (!world) return;

    // Check if at intersection BEFORE moving
    if (isAtIntersection()) {
        // For now: simple test - try to go UP if possible
        Direction newDir = chooseDirectionAtIntersection();
        if (newDir != Direction::NONE) {
            currentDirection = newDir;
            std::cout << "Ghost at intersection, changing to direction: "
                      << static_cast<int>(currentDirection) << std::endl;
        }
    }

    Position dirVec = getDirectionVector(currentDirection);
    Position movement = dirVec * speed * deltaTime;

    // Try horizontal
    if (currentDirection == Direction::LEFT || currentDirection == Direction::RIGHT) {
        Position testPos = Position(position.x + movement.x, position.y);
        if (!world->wouldCollideWithWall(testPos, getCollisionRadius())) {
            position = testPos;
        } else {
            // Hit wall - force direction change
            handleWallCollision();
        }
    }

    // Try vertical
    if (currentDirection == Direction::UP || currentDirection == Direction::DOWN) {
        Position testPos = Position(position.x, position.y + movement.y);
        if (!world->wouldCollideWithWall(testPos, getCollisionRadius())) {
            position = testPos;
        } else {
            // Hit wall - force direction change
            handleWallCollision();
        }
    }
}

bool Ghost::isAtIntersection() const {
    if (!world) return false;

    // Count how many directions are viable (not blocked by walls)
    int viableCount = 0;

    std::vector<Direction> allDirections = {
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT
    };

    for (Direction dir : allDirections) {
        if (world->canMoveInDirection(position, dir, getCollisionRadius())) {
            viableCount++;
        }
    }

    // Intersection = more than 2 viable directions
    // (2 = corridor, 3+ = intersection/T-junction)
    return viableCount >= 3;
}

Direction Ghost::chooseDirectionAtIntersection() {
    if (!world) return Direction::NONE;

    // Get all viable directions
    std::vector<Direction> viable;

    std::vector<Direction> allDirections = {
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT
    };

    for (Direction dir : allDirections) {
        // Don't go 180° backwards
        if (isOpposite(dir, currentDirection)) {
            continue;
        }

        if (world->canMoveInDirection(position, dir, getCollisionRadius())) {
            viable.push_back(dir);
        }
    }

    if (viable.empty()) {
        return currentDirection;  // Keep going
    }

    // SIMPLE TEST: Try to go UP if possible
    for (Direction dir : viable) {
        if (dir == Direction::UP) {
            return Direction::UP;
        }
    }

    // If UP not possible, choose first viable
    return viable[0];
}

void Ghost::handleWallCollision() {
    if (!world) return;

    std::cout << "Ghost hit wall!" << std::endl;

    // Get viable directions (no 180° turn)
    std::vector<Direction> viable;

    std::vector<Direction> allDirections = {
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT
    };

    for (Direction dir : allDirections) {
        // Don't go backwards
        if (isOpposite(dir, currentDirection)) {
            continue;
        }

        if (world->canMoveInDirection(position, dir, getCollisionRadius())) {
            viable.push_back(dir);
        }
    }

    if (!viable.empty()) {
        // Pick random direction
        int index = Random::getInstance().getInt(0, viable.size() - 1);
        currentDirection = viable[index];
        std::cout << "Ghost chose new direction: " << static_cast<int>(currentDirection) << std::endl;
    }
}

bool Ghost::isOpposite(Direction dir1, Direction dir2) const {
    return (dir1 == Direction::UP && dir2 == Direction::DOWN) ||
           (dir1 == Direction::DOWN && dir2 == Direction::UP) ||
           (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) ||
           (dir1 == Direction::RIGHT && dir2 == Direction::LEFT);
}

} // namespace pacman