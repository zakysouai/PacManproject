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
    
    // Lives
    int getLives() const { return lives; }
    void loseLife();
    void reset(const Position& startPos);
    
    // State
    bool isAlive() const { return lives > 0; }
    
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
    float getCollisionRadius() const override { return 0.06f; }
    
private:
    Direction currentDirection = Direction::NONE;
    Direction nextDirection = Direction::NONE;
    int lives = 3;
    Position startPosition;
};

} // namespace pacman