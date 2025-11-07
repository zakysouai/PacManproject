#pragma once
#include "logic/utils/Types.h"
#include <cmath>

namespace pacman {

struct Position {
    float x = 0.0f;
    float y = 0.0f;

    Position() = default;
    Position(float x, float y) : x(x), y(y) {}

    // Manhattan distance
    float manhattanDistance(const Position& other) const {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }

    // Euclidean distance
    float distance(const Position& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    // Operators
    Position operator+(const Position& other) const {
        return Position(x + other.x, y + other.y);
    }

    Position operator-(const Position& other) const {
        return Position(x - other.x, y - other.y);
    }

    Position operator*(float scalar) const {
        return Position(x * scalar, y * scalar);
    }

    bool operator==(const Position& other) const {
        return std::abs(x - other.x) < 0.001f && std::abs(y - other.y) < 0.001f;
    }
};

// Helper function to get direction vector
inline Position getDirectionVector(Direction dir) {
    switch(dir) {
    case Direction::UP:    return Position(0, -1);
    case Direction::DOWN:  return Position(0, 1);
    case Direction::LEFT:  return Position(-1, 0);
    case Direction::RIGHT: return Position(1, 0);
    default:               return Position(0, 0);
    }
}

} // namespace pacman