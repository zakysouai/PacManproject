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
    
    float getCollisionRadius() const override { return 0.04f; }
    
private:
    Direction currentDirection = Direction::NONE;
    Direction nextDirection = Direction::NONE;
    int lives = 3;
    Position startPosition;
};

} // namespace pacman