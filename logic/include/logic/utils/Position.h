#pragma once
#include "logic/utils/Types.h"
#include <cmath>

namespace pacman {

/**
 * @brief 2D positie in world coordinates
 *
 * Gebruikt in:
 * - Entity positions (genormaliseerd [-worldWidth, worldWidth] x [-1, 1])
 * - Movement vectors (direction * speed * deltaTime)
 * - Distance calculations (Manhattan voor AI, Euclidean voor collision)
 */
struct Position {
    float x = 0.0f;
    float y = 0.0f;

    Position() = default;
    Position(float x, float y) : x(x), y(y) {}

    /**
     * @brief Manhattan distance (|dx| + |dy|)
     * @return Distance in world units
     *
     * Gebruikt voor ghost AI pathfinding.
     * Manhattan is sneller dan Euclidean (geen sqrt).
     */
    float manhattanDistance(const Position& other) const {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }

    /**
     * @brief Euclidean distance (sqrt(dx² + dy²))
     * @return Distance in world units
     *
     * Gebruikt voor collision detection.
     */
    float distance(const Position& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    // Vector operaties
    Position operator+(const Position& other) const {
        return Position(x + other.x, y + other.y);
    }

    Position operator-(const Position& other) const {
        return Position(x - other.x, y - other.y);
    }

    Position operator*(float scalar) const {
        return Position(x * scalar, y * scalar);
    }

    /**
     * @brief Equality met epsilon (floating point vergelijking)
     */
    bool operator==(const Position& other) const {
        return std::abs(x - other.x) < 0.001f && std::abs(y - other.y) < 0.001f;
    }
};

/**
 * @brief Converteer Direction naar unit vector
 * @param dir Direction enum
 * @return Unit vector in world space
 *
 * Gebruikt voor movement:
 * newPos = pos + getDirectionVector(dir) * speed * deltaTime
 */
inline Position getDirectionVector(Direction dir) {
    switch(dir) {
    case Direction::UP:    return Position(0, -1);   // Y negatief = omhoog
    case Direction::DOWN:  return Position(0, 1);
    case Direction::LEFT:  return Position(-1, 0);
    case Direction::RIGHT: return Position(1, 0);
    default:               return Position(0, 0);
    }
}

} // namespace pacman