#pragma once
#include "logic/EntityModel.h"

namespace pacman {

class PacMan : public EntityModel {
public:
    explicit PacMan(const Position& pos);
    ~PacMan() override = default;
    
    void update(float deltaTime) override;
    
    void setDirection(Direction dir);
    Direction getDirection() const { return currentDirection; }
    Direction getNextDirection() const { return nextDirection; }
    void tryChangeDirection(Direction newDir);

    int getLives() const { return lives; }
    void loseLife();
    void reset(const Position& startPos);

    bool isAlive() const { return lives > 0; }
    void notifyLevelComplete(int levelBonus);

private:
    Direction currentDirection = Direction::NONE;
    Direction nextDirection = Direction::NONE;
    int lives = 3;
    Position startPosition;
};

} // namespace pacman