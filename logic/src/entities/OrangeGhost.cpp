#include "logic/entities/OrangeGhost.h"
#include "logic/World.h"
#include "logic/utils/Random.h"

namespace pacman {

OrangeGhost::OrangeGhost(World& world, const Position& pos)
    : Ghost(world, pos, GhostColor::ORANGE, 10.0f) {
}

Direction OrangeGhost::chooseDirection() {
    if (!isAtIntersection()) return lockedDirection;

    if (Random::getInstance().getBool(0.5f)) {
        auto viable = getViableDirections();
        if (!viable.empty()) {
            int index = Random::getInstance().getInt(0, viable.size() - 1);
            lockedDirection = viable[index];
        }
    }

    return lockedDirection;
}

} // namespace pacman