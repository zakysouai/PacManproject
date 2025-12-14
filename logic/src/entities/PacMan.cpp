#include "logic/entities/PacMan.h"
#include "logic/utils/Stopwatch.h"

namespace pacman {

PacMan::PacMan(const Position& pos)
    : EntityModel(pos, 0.4f), startPosition(pos) {
}

void PacMan::update(float deltaTime) {
    // Movement handled by World

    // ✅ NOTIFY observers dat entity updated is
    Event event;
    event.type = EventType::ENTITY_UPDATED;
    event.deltaTime = deltaTime;
    notify(event);
}

void PacMan::setDirection(Direction dir) {
    if (dir != nextDirection && dir != Direction::NONE) {
        nextDirection = dir;
    }
}

void PacMan::tryChangeDirection(Direction newDir) {
    if (newDir != currentDirection && newDir != Direction::NONE) {
        currentDirection = newDir;

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

void PacMan::notifyLevelComplete(int levelBonus) {
    Event event;
    event.type = EventType::LEVEL_CLEARED;
    event.value = levelBonus;
    notify(event);
}

} // namespace pacman