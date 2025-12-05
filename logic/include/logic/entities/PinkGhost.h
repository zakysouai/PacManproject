// logic/include/logic/entities/PinkGhost.h
#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

/**
 * @brief Pinky (Pink Ghost) - Predictor
 *
 * Targets the position ahead of Pac-Man based on his current direction.
 * Minimizes Manhattan distance to where Pac-Man is heading.
 */
class PinkGhost : public Ghost {
public:
    explicit PinkGhost(const Position& pos);
    ~PinkGhost() override = default;

protected:
    Direction chooseDirection() override;

private:
    const float PREDICT_DISTANCE = 0.4f;  // How far ahead to target
};

} // namespace pacman