#include "logic/entities/PinkGhost.h"
#include "logic/World.h"

namespace pacman {

PinkGhost::PinkGhost(World& world, const Position& pos)  // ✅ World& parameter
    : Ghost(world, pos, GhostColor::PINK, 0.0f) {  // ✅ world doorgeven
}

Direction PinkGhost::chooseDirection() {
    // ❌ if (!world) return currentDirection; WEG
    if (!isAtIntersection()) return currentDirection;

    auto* pacman = world.getPacMan();  // world. niet world->
    if (!pacman) {
        return currentDirection;
    }

    Position pacmanPos = pacman->getPosition();
    Direction pacmanDir = pacman->getDirection();

    Position dirVec = getDirectionVector(pacmanDir);
    Position target = pacmanPos + dirVec * PREDICT_DISTANCE;

    return getBestDirectionToTarget(target, false);
}

} // namespace pacman