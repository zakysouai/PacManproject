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
    ON_MAP,
    SCARED  // ✅ NIEUW
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
    void setNormalSpeed(float speed) { normalSpeed = speed; }

    bool hasPassedThroughDoor() const { return hasPassedDoor; }
    void markPassedDoor() { hasPassedDoor = true; }

    // ✅ NIEUW: Scared mode
    void enterScaredMode(float duration);
    bool isScared() const { return state == GhostState::SCARED; }
    void respawn();
    void die();

protected:
    virtual Direction chooseDirection() = 0;

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

    // ✅ NIEUW: Scared mode
    float scaredTimer = 0.0f;
    float normalSpeed = 0.3f;  // Original speed
    GhostState previousState = GhostState::ON_MAP;  // State voor scared mode
    float initialSpawnDelay = 0.0f;

    void move(float deltaTime);
    void handleWallCollision();
    bool isOpposite(Direction dir1, Direction dir2) const;
    float calculateManhattanDistance(const Position& from, const Position& to) const;
    Direction chooseDirectionAtIntersection();
    Direction getOppositeDirection(Direction dir) const;
};

} // namespace pacman