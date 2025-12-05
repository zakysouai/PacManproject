// logic/include/logic/entities/OrangeGhost.h
#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

/**
 * @brief Clyde (Orange Ghost) - Locked Direction
 *
 * Moves in a locked direction. At intersections, with 50% probability
 * it locks to a random viable direction and continues in that direction.
 */
class OrangeGhost : public Ghost {
public:
    explicit OrangeGhost(const Position& pos);
    ~OrangeGhost() override = default;

protected:
    Direction chooseDirection() override;

private:
    Direction lockedDirection = Direction::RIGHT;  // Initial direction
};

} // namespace pacman