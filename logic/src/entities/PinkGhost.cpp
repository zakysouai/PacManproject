#include "logic/entities/PinkGhost.h"

namespace pacman {

PinkGhost::PinkGhost(const Position& pos)
    : Ghost(pos, 0.0f) {  // Leaves immediately
}

Direction PinkGhost::chooseDirection([[maybe_unused]] const Position& pacmanPos, Direction pacmanDir) {
    if (!isAtIntersection()) return currentDirection;

    // Target position AHEAD of PacMan
    Position dirVec = getDirectionVector(pacmanDir);
    Position target = pacmanPos + dirVec * PREDICTION_DISTANCE;

    return getBestDirection(target, state == GhostState::FEAR);
}

} // namespace pacman