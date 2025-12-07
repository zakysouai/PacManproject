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
    ENTITY_UPDATED,      // ✅ NIEUW - voor view updates
    COIN_COLLECTED,
    FRUIT_COLLECTED,
    GHOST_EATEN,
    PACMAN_DIED,
    LEVEL_CLEARED,
    DIRECTION_CHANGED,
    GHOST_STATE_CHANGED
};

struct Event {
    EventType type;
    int value = 0;
    float deltaTime = 0.0f;  // ✅ NIEUW - voor animaties
};

} // namespace pacman