// logic/src/entities/OrangeGhost.cpp
#include "logic/entities/OrangeGhost.h"
#include "logic/World.h"
#include "logic/utils/Random.h"

namespace pacman {

OrangeGhost::OrangeGhost(const Position& pos)
    : Ghost(pos, GhostColor::ORANGE, 10.0f) {  // 10 second delay
}

Direction OrangeGhost::chooseDirection() {
    if (!world) return lockedDirection;
    if (!isAtIntersection()) return lockedDirection;

    // At intersection: p=0.5 choose new random direction
    if (Random::getInstance().getBool(0.5f)) {
        auto viable = getViableDirections();
        if (!viable.empty()) {
            int index = Random::getInstance().getInt(0, viable.size() - 1);
            lockedDirection = viable[index];
        }
    }
    // Else: keep current lockedDirection

    return lockedDirection;
}

} // namespace pacman