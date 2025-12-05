#pragma once
#include "logic/EntityModel.h"

namespace pacman {

enum class GhostColor {
    RED,
    PINK,
    BLUE,
    ORANGE
};

enum class GhostState {
    IN_SPAWN,
    ON_MAP
};

class Ghost : public EntityModel {
public:
    explicit Ghost(const Position& pos, GhostColor color);
    ~Ghost() override = default;

    void update(float deltaTime) override;

    GhostColor getColor() const { return color; }
    GhostState getState() const { return state; }
    Direction getCurrentDirection() const { return currentDirection; }

    void setWorld(class World* world) { this->world = world; }
    float getCollisionRadius() const override { return 0.08f; }

    bool hasPassedThroughDoor() const { return hasPassedDoor; }
    void markPassedDoor() { hasPassedDoor = true; }

private:
    GhostColor color;
    GhostState state = GhostState::IN_SPAWN;
    Direction currentDirection = Direction::RIGHT;
    Position spawnPosition;
    World* world = nullptr;

    float spawnTimer = 0.0f;

    void move(float deltaTime);
    bool isAtIntersection() const;
    Direction chooseDirectionAtIntersection();
    void handleWallCollision();
    bool isOpposite(Direction dir1, Direction dir2) const;

    bool hasPassedDoor = false;

    // Red ghost AI
    Direction chooseRedGhostDirection();
    float calculateManhattanDistance(const Position& from, const Position& to) const;
};

} // namespace pacman