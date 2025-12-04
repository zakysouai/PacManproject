#include "logic/entities/Ghost.h"
#include "logic/World.h"
#include "logic/utils/Stopwatch.h"
#include "logic/utils/Random.h"
#include <algorithm>

namespace pacman {

Ghost::Ghost(const Position& pos, float spawnDelay)
    : EntityModel(pos, 0.4f), spawnPosition(pos), spawnDelay(spawnDelay) {
    spawnTimer = spawnDelay;
}

void Ghost::update(float deltaTime) {
    switch (state) {
    case GhostState::SPAWNING:
        updateSpawning(deltaTime);
        break;
    case GhostState::CHASING:
        updateChasing(deltaTime);
        break;
    case GhostState::FEAR:
        updateFear(deltaTime);
        break;
    }
}

void Ghost::updateSpawning(float deltaTime) {
    spawnTimer -= deltaTime;
    if (spawnTimer <= 0.0f) {
        setState(GhostState::CHASING);
    }
}

void Ghost::updateChasing(float deltaTime) {
    move(deltaTime);
}

void Ghost::updateFear(float deltaTime) {
    fearTimer -= deltaTime;
    if (fearTimer <= 0.0f) {
        setState(GhostState::CHASING);
    }
    move(deltaTime);
}

void Ghost::move(float deltaTime) {
    if (!world || currentDirection == Direction::NONE) return;

    Position dirVector = getDirectionVector(currentDirection);
    float currentSpeed = (state == GhostState::FEAR) ? fearSpeed : baseSpeed;
    Position movement = dirVector * currentSpeed * deltaTime;

    // Per-axis collision with AABB
    if (currentDirection == Direction::LEFT || currentDirection == Direction::RIGHT) {
        Position testPos = Position(position.x + movement.x, position.y);
        if (!world->wouldCollideWithWall(testPos, getCollisionRadius())) {
            position = testPos;
        }
    } else if (currentDirection == Direction::UP || currentDirection == Direction::DOWN) {
        Position testPos = Position(position.x, position.y + movement.y);
        if (!world->wouldCollideWithWall(testPos, getCollisionRadius())) {
            position = testPos;
        }
    }
}

void Ghost::setState(GhostState newState) {
    if (state == newState) return;

    state = newState;

    if (state == GhostState::CHASING) {
        speed = baseSpeed;
    } else if (state == GhostState::FEAR) {
        speed = fearSpeed;
        // Reverse direction immediately
        switch (currentDirection) {
        case Direction::UP: currentDirection = Direction::DOWN; break;
        case Direction::DOWN: currentDirection = Direction::UP; break;
        case Direction::LEFT: currentDirection = Direction::RIGHT; break;
        case Direction::RIGHT: currentDirection = Direction::LEFT; break;
        default: break;
        }
    }
}

void Ghost::enterFearMode(float duration) {
    if (state == GhostState::SPAWNING) return;

    setState(GhostState::FEAR);
    fearTimer = duration;
}

void Ghost::respawn() {
    position = spawnPosition;
    currentDirection = Direction::NONE;
    setState(GhostState::CHASING);  // No spawn delay after being eaten
}

bool Ghost::isAtIntersection() const {
    if (!world) return false;
    return world->canMoveInDirection(position, Direction::UP, getCollisionRadius()) ||
           world->canMoveInDirection(position, Direction::DOWN, getCollisionRadius()) ||
           world->canMoveInDirection(position, Direction::LEFT, getCollisionRadius()) ||
           world->canMoveInDirection(position, Direction::RIGHT, getCollisionRadius());
}

std::vector<Direction> Ghost::getViableDirections() const {
    std::vector<Direction> viable;

    if (!world) return viable;

    std::vector<Direction> all = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    for (Direction dir : all) {
        if (world->canMoveInDirection(position, dir, getCollisionRadius())) {
            viable.push_back(dir);
        }
    }

    return viable;
}

Direction Ghost::getBestDirection(const Position& target, bool maximize) const {
    auto viable = getViableDirections();
    if (viable.empty()) return currentDirection;

    Direction best = viable[0];
    float bestDistance = position.manhattanDistance(target);

    for (Direction dir : viable) {
        Position dirVec = getDirectionVector(dir);
        Position testPos = position + dirVec * 0.1f;
        float dist = testPos.manhattanDistance(target);

        if (maximize) {
            if (dist > bestDistance) {
                bestDistance = dist;
                best = dir;
            } else if (std::abs(dist - bestDistance) < 0.001f) {
                if (Random::getInstance().getBool(0.5f)) {
                    best = dir;
                }
            }
        } else {
            if (dist < bestDistance) {
                bestDistance = dist;
                best = dir;
            } else if (std::abs(dist - bestDistance) < 0.001f) {
                if (Random::getInstance().getBool(0.5f)) {
                    best = dir;
                }
            }
        }
    }

    return best;
}

} // namespace pacman