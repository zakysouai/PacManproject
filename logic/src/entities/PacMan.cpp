#include "logic/entities/PacMan.h"
#include "logic/utils/Stopwatch.h"

namespace pacman {

PacMan::PacMan(const Position& pos)
    : EntityModel(pos, 0.5f), startPosition(pos) {
}

void PacMan::update(float deltaTime) {
    // Move in current direction
    if (currentDirection != Direction::NONE) {
        Position dirVector = getDirectionVector(currentDirection);
        position = position + dirVector * speed * deltaTime;
    }
}

void PacMan::setDirection(Direction dir) {
    if (dir != currentDirection) {
        nextDirection = dir;
        // Try to apply immediately, or wait for next intersection
        currentDirection = dir;
        
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
}

} // namespace pacman