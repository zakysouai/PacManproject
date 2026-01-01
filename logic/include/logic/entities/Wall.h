#pragma once
#include "logic/EntityModel.h"

namespace pacman {

/**
 * @brief Wall - collision obstacle
 *
 * Statisch, blokkeert movement van PacMan en ghosts.
 * Collision radius: ~0.49 * tileSize (vrijwel volledige tile)
 */
class Wall : public EntityModel {
public:
    explicit Wall(const Position& pos);
    ~Wall() override = default;

    void update(float deltaTime) override;
};

} // namespace pacman