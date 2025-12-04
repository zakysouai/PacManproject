#pragma once

namespace pacman {

enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
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