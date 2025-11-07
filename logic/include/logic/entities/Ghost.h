#pragma once
#include "logic/EntityModel.h"
#include <memory>

namespace pacman {

class PacMan;  // Forward declaration

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
    
    // Spawning
    void setSpawnDelay(float delay) { spawnTimer = delay; }
    
    // Reset
    void reset(const Position& centerPos);
    void respawn(const Position& centerPos);
    
    float getCollisionRadius() const override { return 0.04f; }
    
private:
    GhostType type;
    GhostMode mode = GhostMode::SPAWNING;
    Direction currentDirection = Direction::RIGHT;
    Direction lockedDirection = Direction::RIGHT;  // For RANDOM type
    
    float spawnTimer = 0.0f;
    float fearTimer = 0.0f;
    float fearDuration = 10.0f;
    
    Position startPosition;
    
    // AI helpers
    Direction chooseRandomDirection();
    Direction chooseChasingDirection(const PacMan& pacman);
    Direction choosePredictorDirection(const PacMan& pacman);
    Direction chooseFearDirection(const PacMan& pacman);
    
    bool isAtIntersection() const;
    std::vector<Direction> getViableDirections() const;
};

} // namespace pacman