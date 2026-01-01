#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

/**
 * @brief Blauwe ghost - Predictive chaser
 *
 * AI Strategie: Target 4 tiles VOOR PacMan (identiek aan PinkGhost).
 * Werkt samen met PinkGhost om PacMan in de tang te nemen.
 *
 * Spawn delay: 5 seconden
 */
class BlueGhost : public Ghost {
public:
    explicit BlueGhost(World& world, const Position& pos);
    ~BlueGhost() override = default;

protected:
    Direction chooseDirection() override;

private:
    const float PREDICT_DISTANCE = 0.4f;  // 4 tiles vooruit
};

} // namespace pacman