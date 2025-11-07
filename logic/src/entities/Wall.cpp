#include "logic/entities/Wall.h"

namespace pacman {

Wall::Wall(const Position& pos)
    : EntityModel(pos, 0.0f) {
}

void Wall::update(float deltaTime) {
    // Walls don't move or do anything
}

} // namespace pacman