// logic/include/logic/entities/RedGhost.h
#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

/**
 * @brief Blinky (Red Ghost) - Pure Chaser
 *
 * Always moves directly toward Pac-Man, choosing the direction
 * that minimizes Manhattan distance to his position.
 */
class RedGhost : public Ghost {
public:
    explicit RedGhost(const Position& pos);
    ~RedGhost() override = default;

protected:
    Direction chooseDirection() override;
};

} // namespace pacman