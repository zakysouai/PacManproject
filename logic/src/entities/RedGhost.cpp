#include "logic/entities/RedGhost.h"
#include "logic/World.h"

namespace pacman {

RedGhost::RedGhost(World& world, const Position& pos)
    : Ghost(world, pos, GhostColor::RED, 0.0f) {
}

Direction RedGhost::chooseDirection() {
    if (!isAtIntersection()) return currentDirection;

    auto* pacman = world.getPacMan();  // world. niet world->
    if (!pacman) {
        return currentDirection;
    }

    Position pacmanPos = pacman->getPosition();
    return getBestDirectionToTarget(pacmanPos, false);
}

} // namespace pacman