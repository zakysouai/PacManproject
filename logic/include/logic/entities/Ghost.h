#pragma once
#include "logic/EntityModel.h"
#include <memory>

namespace pacman {

class PacMan;  // Forward declaration
class World;   // Forward declaration

class Ghost : public EntityModel {
public:
    Ghost(const Position& pos, GhostType type);
    ~Ghost() override = default;

    void update(float deltaTime) override;

    // AI behavior
    void updateAI(const PacMan& pacman, float deltaTime);
    Direction chooseDirection(const PacMan& pacman);

    // Mode management
    void setMode(GhostMode mode);
    GhostMode getMode() const { return mode; }

    // Fear mode
    void enterFearMode(float duration);
    bool isFeared() const { return mode == GhostMode::FEAR; }

    // Type
    GhostType getType() const { return type; }

    // Movement getters (for collision detection and AI)
    Direction getCurrentDirection() const { return currentDirection; }

    // ✅ NEW: Set current direction (for forced direction changes on collision)
    void setCurrentDirection(Direction dir) { currentDirection = dir; }

    // Spawning
    void setSpawnDelay(float delay) { spawnTimer = delay; }

    // Reset
    void reset(const Position& centerPos);
    void respawn(const Position& centerPos);

    // ✅ NEW: Set world pointer for collision checking
    void setWorld(World* worldPtr) { world = worldPtr; }

    // ✅ Smaller radius to fit through narrow spawn exits
    float getCollisionRadius() const override { return 0.09f; }

private:
    GhostType type;
    GhostMode mode = GhostMode::SPAWNING;
    Direction currentDirection = Direction::RIGHT;
    Direction lockedDirection = Direction::RIGHT;  // For RANDOM type

    float spawnTimer = 0.0f;
    float fearTimer = 0.0f;
    float fearDuration = 10.0f;

    Position startPosition;

    // ✅ NEW: World pointer for checking viable directions (non-owning)
    World* world = nullptr;

    // AI helpers
    Direction chooseRandomDirection();
    Direction chooseChasingDirection(const PacMan& pacman);
    Direction choosePredictorDirection(const PacMan& pacman);
    Direction chooseFearDirection(const PacMan& pacman);

    // ✅ UPDATED: Now checks if actually at intersection using world
    bool isAtIntersection() const;

    // ✅ UPDATED: Now filters out blocked directions using world
    std::vector<Direction> getViableDirections() const;

    // Helper to check if two directions are opposite
    bool isOppositeDirection(Direction dir1, Direction dir2) const;
};

} // namespace pacman