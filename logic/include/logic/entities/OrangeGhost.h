#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

/**
 * @brief Oranje ghost - Random movement
 *
 * AI Strategie: Bij elk intersection 50% kans om random direction te kiezen.
 * Minst voorspelbare ghost, voegt chaos toe aan gameplay.
 *
 * Spawn delay: 10 seconden (laatste ghost)
 */
class OrangeGhost : public Ghost {
public:
    explicit OrangeGhost(World& world, const Position& pos);
    ~OrangeGhost() override = default;

protected:
    Direction chooseDirection() override;

private:
    Direction lockedDirection = Direction::RIGHT;
};

} // namespace pacman