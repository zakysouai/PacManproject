#pragma once
#include "logic/EntityModel.h"

namespace pacman {

class PacMan : public EntityModel {
public:
    explicit PacMan(const Position& pos);
    ~PacMan() override = default;
    
    void update(float deltaTime) override;
    
    // Movement
    void setDirection(Direction dir);
    Direction getDirection() const { return currentDirection; }
    Direction getNextDirection() const { return nextDirection; }
    
    /**
     * @brief Attempt to change PacMan's current direction
     *
     * This is called by World when it determines that a direction change
     * is possible (e.g., at an intersection). It updates currentDirection
     * and notifies observers for animation updates.
     *
     * @param newDir The new direction to change to
     */
    void tryChangeDirection(Direction newDir);

    // Lives
    int getLives() const { return lives; }
    void loseLife();
    void reset(const Position& startPos);
    
    // State
    bool isAlive() const { return lives > 0; }
    
    /**
     * @brief Check if PacMan is currently invulnerable
     *
     * After losing a life, PacMan is invulnerable for a short period
     * to prevent losing multiple lives immediately.
     *
     * @return true if invulnerable, false otherwise
     */
    bool isInvulnerable() const { return invulnerabilityTimer > 0.0f; }

    /**
     * @brief Get PacMan's collision radius
     *
     * PacMan's collision radius is set to exactly one tile size.
     * A tile is approximately 0.182 world units (2.0 / 11), so the
     * radius is 0.091 (half a tile). This ensures consistent collision
     * detection and prevents any visual issues with walls.
     *
     * @return Collision radius = 0.091 (exactly half a tile)
     */
    float getCollisionRadius() const override { return 0.08f; }

private:
    Direction currentDirection = Direction::NONE;
    Direction nextDirection = Direction::NONE;
    int lives = 3;
    Position startPosition;

    // Invulnerability after losing a life
    float invulnerabilityTimer = 0.0f;
    const float INVULNERABILITY_DURATION = 2.0f;  // 2 seconds
};

} // namespace pacman