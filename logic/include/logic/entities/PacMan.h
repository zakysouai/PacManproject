#pragma once
#include "logic/EntityModel.h"

namespace pacman {

/**
 * @brief PacMan - de speler
 *
 * Kenmerken:
 * - Input buffering: nextDirection wordt opgeslagen tot intersection
 * - 3 levens bij start
 * - Speed: 0.4 world units/sec
 */
class PacMan : public EntityModel {
public:
    explicit PacMan(const Position& pos);
    ~PacMan() override = default;

    void update(float deltaTime) override;

    /**
     * @brief Stel gewenste direction in (input buffering)
     *
     * Direction wordt pas uitgevoerd bij intersection of als 180° turn.
     */
    void setDirection(Direction dir);

    Direction getDirection() const { return currentDirection; }
    Direction getNextDirection() const { return nextDirection; }

    /**
     * @brief Probeer direction change uit te voeren
     *
     * Alleen succesvol als nieuwe direction geldig is.
     * Triggert DIRECTION_CHANGED event voor animatie switch.
     */
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