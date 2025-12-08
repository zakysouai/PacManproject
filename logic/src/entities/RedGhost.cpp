#include "logic/entities/RedGhost.h"
#include "logic/World.h"

namespace pacman {

RedGhost::RedGhost(World& world, const Position& pos)  // ✅ World& parameter
    : Ghost(world, pos, GhostColor::RED, 0.0f) {  // ✅ world doorgeven
}

Direction RedGhost::chooseDirection() {
    // ❌ if (!world) return currentDirection; WEG
    if (!isAtIntersection()) return currentDirection;

    auto* pacman = world.getPacMan();  // world. niet world->
    if (!pacman) {
        return currentDirection;
    }

    Position pacmanPos = pacman->getPosition();
    return getBestDirectionToTarget(pacmanPos, false);
}

} // namespace pacman