#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

/**
 * @brief Rode ghost - Direct chaser
 *
 * AI Strategie: Minimaliseer Manhattan distance naar PacMan's huidige positie.
 * Meest agressieve ghost, volgt PacMan direct.
 *
 * Spawn delay: 0 seconden (eerste ghost die spawn verlaat)
 */
class RedGhost : public Ghost {
public:
    explicit RedGhost(World& world, const Position& pos);
    ~RedGhost() override = default;

protected:
    Direction chooseDirection() override;
};

} // namespace pacman