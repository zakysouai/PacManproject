#pragma once
#include "logic/EntityModel.h"
#include <memory>

namespace pacman {

class PacMan;  // Forward declaration
class World;   // Forward declaration

// In logic/include/logic/entities/Ghost.h

class Ghost : public EntityModel {
public:
    explicit Ghost(const Position& pos, GhostType type);
    ~Ghost() override = default;

    void update(float deltaTime) override;
    void updateAI(const PacMan& pacman, float deltaTime);
    Direction chooseDirection(const PacMan& pacman);

    void setMode(GhostMode mode);
    GhostMode getMode() const { return mode; }

    // ✅ ADD THESE MISSING METHODS:
    void setExitPosition(const Position& exitPos) { exitPosition = exitPos; }
    Position getExitPosition() const { return exitPosition; }

    void enterFearMode(float duration);
    bool isFeared() const { return mode == GhostMode::FEAR; }

    GhostType getType() const { return type; }
    Direction getCurrentDirection() const { return currentDirection; }
    void setCurrentDirection(Direction dir) { currentDirection = dir; }

    void setSpawnDelay(float delay) { spawnTimer = delay; }
    void reset(const Position& centerPos);
    void respawn(const Position& centerPos);

    void setWorld(World* worldPtr) { world = worldPtr; }
    float getCollisionRadius() const override { return 0.085f; }

    // ✅ ADD THIS METHOD:
    bool hasLeftSpawn() const;

private:
    GhostType type;
    GhostMode mode = GhostMode::SPAWNING;
    Direction currentDirection = Direction::UP;
    Direction lockedDirection = Direction::RIGHT;

    float spawnTimer = 0.0f;
    float fearTimer = 0.0f;
    float fearDuration = 10.0f;

    Position startPosition;
    Position exitPosition;  // This exists but setter was missing
    World* world = nullptr;

    bool hasExitedSpawn = false;

    // ✅ ADD THESE MISSING METHOD DECLARATIONS:
    Direction getDirectionToTarget(const Position& target) const;
    Direction chooseSpawnExitDirection();
    Direction chooseRandomDirection();
    Direction chooseChasingDirection(const PacMan& pacman);
    Direction choosePredictorDirection(const PacMan& pacman);
    Direction chooseFearDirection(const PacMan& pacman);

    bool isAtIntersection() const;
    std::vector<Direction> getViableDirections() const;
    bool isOppositeDirection(Direction dir1, Direction dir2) const;
};

} // namespace pacman