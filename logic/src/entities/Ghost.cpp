#include "logic/entities/Ghost.h"
#include "logic/World.h"
#include "logic/utils/Random.h"
#include <iostream>

namespace pacman {

Ghost::Ghost(const Position& pos, GhostColor color, float spawnDelay)
    : EntityModel(pos, 0.3f), color(color), spawnPosition(pos), spawnTimer(spawnDelay) {
    // ✅ spawnDelay komt nu van subclass, niet meer via switch
}

void Ghost::update(float deltaTime) {
    // Handle spawn timer
    if (state == GhostState::IN_SPAWN) {
        spawnTimer -= deltaTime;
        if (spawnTimer <= 0.0f) {
            state = GhostState::ON_MAP;
            std::cout << "Ghost color " << static_cast<int>(color) << " left spawn!" << std::endl;
        }
        return;
    }

    move(deltaTime);
}

void Ghost::move(float deltaTime) {
    if (!world) return;

    // Check if at intersection BEFORE moving
    if (isAtIntersection()) {
        Direction newDir = chooseDirectionAtIntersection();
        if (newDir != Direction::NONE && newDir != currentDirection) {
            currentDirection = newDir;
        }
    }

    Position dirVec = getDirectionVector(currentDirection);
    Position movement = dirVec * speed * deltaTime;

    // Try horizontal movement
    if (currentDirection == Direction::LEFT || currentDirection == Direction::RIGHT) {
        Position testPos = Position(position.x + movement.x, position.y);
        if (!world->wouldCollideWithWall(testPos, getCollisionRadius(), this)) {
            position = testPos;
        } else {
            handleWallCollision();
        }
    }

    // Try vertical movement
    if (currentDirection == Direction::UP || currentDirection == Direction::DOWN) {
        Position testPos = Position(position.x, position.y + movement.y);
        if (!world->wouldCollideWithWall(testPos, getCollisionRadius(), this)) {
            position = testPos;
        } else {
            handleWallCollision();
        }
    }

    // ✅ SIMPEL: check of ghost tile boven deur bereikt
    if (!hasPassedDoor && world->hasDoorInMap()) {
        auto ghostGrid = world->worldToGrid(position);
        auto doorGrid = world->getDoorGridPosition();

        if (ghostGrid.row == doorGrid.row - 1 && ghostGrid.col == doorGrid.col) {
            markPassedDoor();
            std::cout << "Ghost " << static_cast<int>(color) << " PASSED door!" << std::endl;
        }
    }
}

bool Ghost::isAtIntersection() const {
    if (!world) return false;

    int viableCount = 0;
    std::vector<Direction> allDirections = {
        Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT
    };

    for (Direction dir : allDirections) {
        // ✅ PASS THIS POINTER
        if (world->canMoveInDirection(position, dir, getCollisionRadius(), this)) {
            viableCount++;
        }
    }

    return viableCount >= 3;
}

Direction Ghost::chooseDirectionAtIntersection() {
    // ✅ Roep virtual method aan
    return chooseDirection();
}

float Ghost::calculateManhattanDistance(const Position& from, const Position& to) const {
    return std::abs(from.x - to.x) + std::abs(from.y - to.y);
}

void Ghost::handleWallCollision() {
    if (!world) return;

    std::vector<Direction> viable;
    std::vector<Direction> allDirections = {
        Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT
    };

    for (Direction dir : allDirections) {
        if (isOpposite(dir, currentDirection)) continue;
        // ✅ PASS THIS POINTER
        if (world->canMoveInDirection(position, dir, getCollisionRadius(), this)) {
            viable.push_back(dir);
        }
    }

    if (!viable.empty()) {
        int index = Random::getInstance().getInt(0, viable.size() - 1);
        currentDirection = viable[index];
    } else {
        switch (currentDirection) {
        case Direction::UP: currentDirection = Direction::DOWN; break;
        case Direction::DOWN: currentDirection = Direction::UP; break;
        case Direction::LEFT: currentDirection = Direction::RIGHT; break;
        case Direction::RIGHT: currentDirection = Direction::LEFT; break;
        default: break;
        }
    }
}

bool Ghost::isOpposite(Direction dir1, Direction dir2) const {
    return (dir1 == Direction::UP && dir2 == Direction::DOWN) ||
           (dir1 == Direction::DOWN && dir2 == Direction::UP) ||
           (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) ||
           (dir1 == Direction::RIGHT && dir2 == Direction::LEFT);
}

std::vector<Direction> Ghost::getViableDirections() const {
    if (!world) return {};

    std::vector<Direction> viable;
    std::vector<Direction> allDirections = {
        Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT
    };

    for (Direction dir : allDirections) {
        if (isOpposite(dir, currentDirection)) continue;
        if (world->canMoveInDirection(position, dir, getCollisionRadius(), this)) {
            viable.push_back(dir);
        }
    }

    return viable;
}

Direction Ghost::getBestDirectionToTarget(const Position& target, bool maximize) const {
    auto viable = getViableDirections();
    if (viable.empty()) return currentDirection;

    Direction bestDir = viable[0];
    float bestDistance = maximize ? -999999.0f : 999999.0f;

    for (Direction dir : viable) {
        Position dirVec = getDirectionVector(dir);
        Position testPos = position + dirVec * 0.1f;
        float dist = calculateManhattanDistance(testPos, target);

        if (maximize) {
            if (dist > bestDistance) {
                bestDistance = dist;
                bestDir = dir;
            }
        } else {
            if (dist < bestDistance) {
                bestDistance = dist;
                bestDir = dir;
            }
        }
    }

    return bestDir;
}

} // namespace pacman