#include "logic/entities/BlueGhost.h"
#include "logic/World.h"

namespace pacman {

BlueGhost::BlueGhost(World& world, const Position& pos)  // ✅ World& parameter
    : Ghost(world, pos, GhostColor::BLUE, 5.0f) {  // ✅ world doorgeven
}

Direction BlueGhost::chooseDirection() {
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