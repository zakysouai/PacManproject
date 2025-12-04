#include "logic/entities/RedGhost.h"
#include "logic/utils/Random.h"

namespace pacman {

RedGhost::RedGhost(const Position& pos)
    : Ghost(pos, 0.0f) {  // Leaves immediately
}

Direction RedGhost::chooseDirection(const Position& pacmanPos, [[maybe_unused]] Direction pacmanDir) {
    if (!isAtIntersection()) return currentDirection;

    auto viable = getViableDirections();
    if (viable.empty()) return currentDirection;

    // Random lock behavior: p=0.5 choose random direction
    if (Random::getInstance().getBool(0.5f)) {
        int index = Random::getInstance().getInt(0, viable.size() - 1);
        return viable[index];
    }

    // Otherwise chase PacMan
    return getBestDirection(pacmanPos, false);
}

} // namespace pacman