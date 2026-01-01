#pragma once

namespace pacman {

/**
 * @brief Movement directions
 */
enum class Direction {
    NONE,   // Geen beweging
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * @brief Event types voor Observer pattern
 */
enum class EventType {
    ENTITY_UPDATED,        // Elke frame, gebruikt voor View updates en animaties
    COIN_COLLECTED,        // value = base points
    FRUIT_COLLECTED,       // value = base points
    GHOST_EATEN,           // value = 200 points
    PACMAN_DIED,           // Trigger death animation
    LEVEL_CLEARED,         // value = level bonus
    DIRECTION_CHANGED,     // Trigger animatie switch (PacMan)
    GHOST_STATE_CHANGED    // Trigger animatie switch (Ghost scared/normal)
};

/**
 * @brief Event data container
 */
struct Event {
    EventType type;
    int value = 0;           // Score value voor collectibles/bonuses
    float deltaTime = 0.0f;  // Tijd sinds laatste frame (voor animaties)
};

} // namespace pacman