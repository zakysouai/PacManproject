#include "logic/entities/Fruit.h"

namespace pacman {

Fruit::Fruit(const Position& pos)
    : EntityModel(pos, 0.0f) {
}

void Fruit::update(float deltaTime) {
    // Fruits don't move
}

void Fruit::collect() {
    if (!collected) {
        collected = true;
        
        Event event;
        event.type = EventType::FRUIT_COLLECTED;
        event.value = pointValue;
        notify(event);
    }
}

} // namespace pacman