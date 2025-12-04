#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

/**
 * @brief Clyde (Orange Ghost) - Random/Locked Direction
 * 
 * Moves in a locked direction. At intersections, with 50% probability
 * it locks to a random viable direction.
 */
class PinkGhost : public Ghost {
public:
    explicit PinkGhost(const Position& pos);
    ~PinkGhost() override = default;
    
protected:
    Direction chooseDirection() override;
    
private:
    Direction lockedDirection = Direction::NONE;
    bool isAtIntersection() const;
};

} // namespace pacman