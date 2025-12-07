#include "logic/entities/Wall.h"

namespace pacman {

Wall::Wall(const Position& pos)
    : EntityModel(pos, 0.0f) {
}

void Wall::update(float deltaTime) {
    // Walls don't move

    // ✅ NOTIFY (voor consistentie)
    Event event;
    event.type = EventType::ENTITY_UPDATED;
    event.deltaTime = deltaTime;
    notify(event);
}

} // namespace pacman