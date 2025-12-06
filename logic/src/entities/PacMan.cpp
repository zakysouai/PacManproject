#include "logic/entities/PacMan.h"
#include "logic/utils/Stopwatch.h"

namespace pacman {

PacMan::PacMan(const Position& pos)
    : EntityModel(pos, 0.5f), startPosition(pos) {
}

void PacMan::update(float deltaTime) {
    // Movement is now handled by World::updatePacManWithCollisions
    // to properly support direction changes at intersections
}

void PacMan::setDirection(Direction dir) {
    // Store the requested direction - it will be applied at the next intersection
    // where this direction is viable
    if (dir != nextDirection && dir != Direction::NONE) {
        nextDirection = dir;
    }
}

void PacMan::tryChangeDirection(Direction newDir) {
    if (newDir != currentDirection && newDir != Direction::NONE) {
        currentDirection = newDir;

        // Notify observers that direction changed (for animation)
        Event event;
        event.type = EventType::DIRECTION_CHANGED;
        notify(event);
    }
}

void PacMan::loseLife() {
    if (lives > 0) {
        lives--;

        Event event;
        event.type = EventType::PACMAN_DIED;
        notify(event);
    }
}

void PacMan::reset(const Position& startPos) {
    position = startPos;
    startPosition = startPos;
    currentDirection = Direction::NONE;
    nextDirection = Direction::NONE;

    // Reset invulnerability timer (will be set by loseLife if called)
    // Don't reset it here to allow invulnerability after respawn
}

} // namespace pacman