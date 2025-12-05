// logic/src/entities/BlueGhost.cpp
#include "logic/entities/BlueGhost.h"
#include "logic/World.h"

namespace pacman {

BlueGhost::BlueGhost(const Position& pos)
    : Ghost(pos, GhostColor::BLUE, 5.0f) {  // ✅ 5 seconds delay
}

Direction BlueGhost::chooseDirection() {
    if (!world) return currentDirection;
    if (!isAtIntersection()) return currentDirection;

    auto* pacman = world->getPacMan();
    if (!pacman) {
        return currentDirection;
    }

    // Target position AHEAD of PacMan
    Position pacmanPos = pacman->getPosition();
    Direction pacmanDir = pacman->getDirection();

    Position dirVec = getDirectionVector(pacmanDir);
    Position target = pacmanPos + dirVec * PREDICT_DISTANCE;

    return getBestDirectionToTarget(target, false);
}

} // namespace pacman