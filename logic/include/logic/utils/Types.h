#pragma once

namespace pacman {

enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class GhostType {
    RANDOM,      // Locks to random direction at intersections
    CHASER,      // Chases PacMan directly
    PREDICTOR    // Moves towards where PacMan is facing
};

enum class GhostMode {
    SPAWNING,    // Waiting in center before leaving
    CHASING,     // Normal mode: chasing PacMan
    FEAR         // Vulnerable mode after eating fruit
};

enum class EventType {
    COIN_COLLECTED,
    FRUIT_COLLECTED,
    GHOST_EATEN,
    PACMAN_DIED,
    LEVEL_CLEARED,
    DIRECTION_CHANGED
};

struct Event {
    EventType type;
    int value = 0;  // Points, extra data, etc.
};

} // namespace pacman