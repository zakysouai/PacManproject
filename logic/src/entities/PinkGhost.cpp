// logic/src/entities/PinkGhost.cpp
#include "logic/entities/PinkGhost.h"
#include "logic/World.h"

namespace pacman {

PinkGhost::PinkGhost(const Position& pos)
    : Ghost(pos, GhostColor::PINK, 0.0f) {  // Leaves immediately
}

Direction PinkGhost::chooseDirection() {
    if (!world) return currentDirection;
    if (!isAtIntersection()) return currentDirection;

    auto* pacman = world->getPacMan();
    if (!pacman) {
        return currentDirection;
    }

    // ✅ PREDICTOR: target position AHEAD of PacMan
    Position pacmanPos = pacman->getPosition();
    Direction pacmanDir = pacman->getDirection();

    // Calculate target position
    Position dirVec = getDirectionVector(pacmanDir);
    Position target = pacmanPos + dirVec * PREDICT_DISTANCE;

    // Minimize distance to predicted position
    return getBestDirectionToTarget(target, false);
}

} // namespace pacman