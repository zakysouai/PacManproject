// logic/include/logic/entities/Ghost.h
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
    explicit Ghost(const Position& pos, GhostColor color, float spawnDelay);
    virtual ~Ghost() = default;

    void update(float deltaTime) override;

    GhostColor getColor() const { return color; }
    GhostState getState() const { return state; }
    Direction getCurrentDirection() const { return currentDirection; }

    void setWorld(class World* world) { this->world = world; }
    float getCollisionRadius() const override { return 0.08f; }

    bool hasPassedThroughDoor() const { return hasPassedDoor; }
    void markPassedDoor() { hasPassedDoor = true; }

protected:
    // ✅ NIEUW: Virtual AI method - subclasses implementeren dit
    virtual Direction chooseDirection() = 0;

    // Helper methods beschikbaar voor subclasses
    bool isAtIntersection() const;
    std::vector<Direction> getViableDirections() const;
    Direction getBestDirectionToTarget(const Position& target, bool maximize = false) const;

    World* world = nullptr;
    Direction currentDirection = Direction::RIGHT;

private:
    GhostColor color;
    GhostState state = GhostState::IN_SPAWN;
    Position spawnPosition;

    float spawnTimer = 0.0f;
    bool hasPassedDoor = false;

    void move(float deltaTime);
    void handleWallCollision();
    bool isOpposite(Direction dir1, Direction dir2) const;
    float calculateManhattanDistance(const Position& from, const Position& to) const;
    Direction chooseDirectionAtIntersection();
};

} // namespace pacman