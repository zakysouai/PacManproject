#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

/**
 * @brief Roze ghost - Predictive chaser
 *
 * AI Strategie: Target 4 tiles VOOR PacMan in zijn huidige bewegingsrichting.
 * Probeert PacMan af te snijden ipv direct te volgen.
 *
 * Spawn delay: 0 seconden (tweede ghost die spawn verlaat)
 */
class PinkGhost : public Ghost {
public:
    explicit PinkGhost(World& world, const Position& pos);
    ~PinkGhost() override = default;

protected:
    Direction chooseDirection() override;

private:
    const float PREDICT_DISTANCE = 0.4f;  // 4 tiles vooruit
};

} // namespace pacman