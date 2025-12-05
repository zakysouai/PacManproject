// logic/include/logic/entities/BlueGhost.h
#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

/**
 * @brief Inky (Blue/Cyan Ghost) - Predictor with Delay
 *
 * Similar to Pinky, targets ahead of Pac-Man's current direction.
 * Waits 5 seconds before leaving spawn.
 */
class BlueGhost : public Ghost {
public:
    explicit BlueGhost(const Position& pos);
    ~BlueGhost() override = default;

protected:
    Direction chooseDirection() override;

private:
    const float PREDICT_DISTANCE = 0.4f;
};

} // namespace pacman