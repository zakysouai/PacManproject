#pragma once
#include "logic/EntityModel.h"
#include "logic/entities/PacMan.h"
#include <memory>

namespace pacman {

enum class GhostState {
    SPAWNING,
    CHASING,
    FEAR
};

class Ghost : public EntityModel {
public:
    explicit Ghost(const Position& pos, float spawnDelay);
    ~Ghost() override = default;

    void update(float deltaTime) override;

    // State management
    GhostState getState() const { return state; }
    void setState(GhostState newState);
    void enterFearMode(float duration);
    void respawn();

    // AI
    virtual Direction chooseDirection(const Position& pacmanPos, Direction pacmanDir) = 0;

    // Getters
    float getCollisionRadius() const override { return 0.08f; }
    Direction getCurrentDirection() const { return currentDirection; }
    bool isInFearMode() const { return state == GhostState::FEAR; }

    // Setters
    void setWorld(class World* world) { this->world = world; }

protected:
    GhostState state = GhostState::SPAWNING;
    Direction currentDirection = Direction::NONE;

    Position spawnPosition;
    float spawnTimer;
    float spawnDelay;

    float fearTimer = 0.0f;
    float baseSpeed = 0.4f;
    float fearSpeed = 0.2f;

    World* world = nullptr;  // Non-owning pointer

    // Helper methods
    bool isAtIntersection() const;
    std::vector<Direction> getViableDirections() const;
    Direction getBestDirection(const Position& target, bool maximize = false) const;

private:
    void updateSpawning(float deltaTime);
    void updateChasing(float deltaTime);
    void updateFear(float deltaTime);
    void move(float deltaTime);
};

} // namespace pacman