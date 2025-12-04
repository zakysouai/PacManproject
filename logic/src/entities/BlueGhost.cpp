#include "logic/entities/BlueGhost.h"

namespace pacman {

BlueGhost::BlueGhost(const Position& pos)
    : Ghost(pos, 5.0f) {  // 5 second delay
}

Direction BlueGhost::chooseDirection([[maybe_unused]] const Position& pacmanPos, Direction pacmanDir) {
    if (!isAtIntersection()) return currentDirection;

    // Target position AHEAD of PacMan
    Position dirVec = getDirectionVector(pacmanDir);
    Position target = pacmanPos + dirVec * PREDICTION_DISTANCE;

    return getBestDirection(target, state == GhostState::FEAR);
}

} // namespace pacman