#include "logic/entities/OrangeGhost.h"

namespace pacman {

OrangeGhost::OrangeGhost(const Position& pos)
    : Ghost(pos, 10.0f) {  // 10 second delay
}

Direction OrangeGhost::chooseDirection(const Position& pacmanPos, [[maybe_unused]] Direction pacmanDir) {
    if (!isAtIntersection()) return currentDirection;

    // Direct chaser - minimize distance to PacMan
    return getBestDirection(pacmanPos, state == GhostState::FEAR);
}

} // namespace pacman