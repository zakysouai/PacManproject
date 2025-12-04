#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

/**
 * @brief Inky (Cyan Ghost) - Predictor
 * 
 * Similar to Pinky, targets ahead of Pac-Man's current direction.
 * Minimizes Manhattan distance to predicted position.
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