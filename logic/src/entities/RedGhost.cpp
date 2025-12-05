// logic/src/entities/RedGhost.cpp
#include "logic/entities/RedGhost.h"
#include "logic/World.h"

namespace pacman {

RedGhost::RedGhost(const Position& pos)
    : Ghost(pos, GhostColor::RED, 0.0f) {  // Leaves immediately
}

Direction RedGhost::chooseDirection() {
    if (!world) return currentDirection;
    if (!isAtIntersection()) return currentDirection;

    auto* pacman = world->getPacMan();
    if (!pacman) {
        // No PacMan? Keep current direction
        return currentDirection;
    }

    // ✅ PURE CHASER: minimize distance to PacMan's actual position
    Position pacmanPos = pacman->getPosition();
    return getBestDirectionToTarget(pacmanPos, false);  // false = minimize
}

} // namespace pacman