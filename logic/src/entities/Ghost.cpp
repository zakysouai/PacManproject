#include "logic/entities/Ghost.h"
#include "logic/World.h"
#include "logic/utils/Random.h"
#include <iostream>

namespace pacman {

Ghost::Ghost(const Position& pos, GhostColor color, float spawnDelay)
    : EntityModel(pos, 0.3f), color(color), spawnPosition(pos), spawnTimer(spawnDelay) {
    normalSpeed = 0.3f;
}

void Ghost::update(float deltaTime) {
    // Handle spawn timer
    if (state == GhostState::IN_SPAWN) {
        spawnTimer -= deltaTime;
        if (spawnTimer <= 0.0f) {
            state = GhostState::ON_MAP;
            std::cout << "Ghost color " << static_cast<int>(color) << " left spawn!" << std::endl;

            Event event;
            event.type = EventType::GHOST_STATE_CHANGED;
            notify(event);
        }
        // ✅ NOTIFY zelfs in spawn
        Event updateEvent;
        updateEvent.type = EventType::ENTITY_UPDATED;
        updateEvent.deltaTime = deltaTime;
        notify(updateEvent);
        return;
    }

    // Handle scared timer
    if (state == GhostState::SCARED) {
        scaredTimer -= deltaTime;
        if (scaredTimer <= 0.0f) {
            state = previousState;
            speed = normalSpeed;
            std::cout << "Ghost color " << static_cast<int>(color) << " exits scared mode!" << std::endl;

            Event event;
            event.type = EventType::GHOST_STATE_CHANGED;
            notify(event);
        }
    }

    move(deltaTime);

    // ✅ NOTIFY observers
    Event event;
    event.type = EventType::ENTITY_UPDATED;
    event.deltaTime = deltaTime;
    notify(event);
}

void Ghost::enterScaredMode(float duration) {
    if (state == GhostState::IN_SPAWN) {
        return;  // Ghosts in spawn don't get scared
    }

    previousState = state;
    state = GhostState::SCARED;
    scaredTimer = duration;
    speed = normalSpeed * 0.5f;  // ✅ Slower when scared

    // ✅ REVERSE direction immediately
    currentDirection = getOppositeDirection(currentDirection);

    std::cout << "Ghost color " << static_cast<int>(color) << " enters scared mode!" << std::endl;

    Event event;
    event.type = EventType::GHOST_STATE_CHANGED;
    notify(event);
}

void Ghost::respawn() {
    position = spawnPosition;
    currentDirection = Direction::RIGHT;  // ✅ Reset direction
    state = GhostState::ON_MAP;
    speed = normalSpeed;
    hasPassedDoor = false;
    scaredTimer = 0.0f;

    std::cout << "Ghost color " << static_cast<int>(color) << " respawned!" << std::endl;

    Event event;
    event.type = EventType::GHOST_STATE_CHANGED;
    notify(event);
}

void Ghost::die() {
    // ✅ NOTIFY observers (Score krijgt event)
    Event event;
    event.type = EventType::GHOST_EATEN;
    event.value = 200;
    notify(event);

    respawn();
}

void Ghost::move(float deltaTime) {
    if (!world) return;

    if (isAtIntersection()) {
        Direction newDir = chooseDirectionAtIntersection();
        if (newDir != Direction::NONE && newDir != currentDirection) {
            currentDirection = newDir;
        }
    }

    Position dirVec = getDirectionVector(currentDirection);
    Position movement = dirVec * speed * deltaTime;

    if (currentDirection == Direction::LEFT || currentDirection == Direction::RIGHT) {
        Position testPos = Position(position.x + movement.x, position.y);
        if (!world->wouldCollideWithWall(testPos, getCollisionRadius(), this)) {
            position = testPos;
        } else {
            handleWallCollision();
        }
    }

    if (currentDirection == Direction::UP || currentDirection == Direction::DOWN) {
        Position testPos = Position(position.x, position.y + movement.y);
        if (!world->wouldCollideWithWall(testPos, getCollisionRadius(), this)) {
            position = testPos;
        } else {
            handleWallCollision();
        }
    }

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
        if (world->canMoveInDirection(position, dir, getCollisionRadius(), this)) {
            viableCount++;
        }
    }

    return viableCount >= 3;
}

Direction Ghost::chooseDirectionAtIntersection() {
    // ✅ SCARED MODE: maximize distance
    if (state == GhostState::SCARED && world && world->getPacMan()) {
        Position pacmanPos = world->getPacMan()->getPosition();
        return getBestDirectionToTarget(pacmanPos, true);  // true = MAXIMIZE
    }

    // Normal mode: use subclass AI
    return chooseDirection();
}

Direction Ghost::getOppositeDirection(Direction dir) const {
    switch (dir) {
        case Direction::UP:    return Direction::DOWN;
        case Direction::DOWN:  return Direction::UP;
        case Direction::LEFT:  return Direction::RIGHT;
        case Direction::RIGHT: return Direction::LEFT;
        default:               return Direction::NONE;
    }
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
        if (world->canMoveInDirection(position, dir, getCollisionRadius(), this)) {
            viable.push_back(dir);
        }
    }

    if (!viable.empty()) {
        int index = Random::getInstance().getInt(0, viable.size() - 1);
        currentDirection = viable[index];
    } else {
        currentDirection = getOppositeDirection(currentDirection);
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