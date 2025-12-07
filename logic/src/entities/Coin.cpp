#include "logic/entities/Coin.h"

namespace pacman {

Coin::Coin(const Position& pos)
    : EntityModel(pos, 0.0f) {
}

void Coin::update(float deltaTime) {
    // Coins don't move

    // ✅ NOTIFY (hoewel statisch, voor consistentie)
    Event event;
    event.type = EventType::ENTITY_UPDATED;
    event.deltaTime = deltaTime;
    notify(event);
}

void Coin::collect() {
    if (!collected) {
        collected = true;
        
        Event event;
        event.type = EventType::COIN_COLLECTED;
        event.value = pointValue;
        notify(event);
    }
}

} // namespace pacman