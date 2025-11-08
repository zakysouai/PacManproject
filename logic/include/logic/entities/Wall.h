#pragma once
#include "logic/EntityModel.h"

namespace pacman {

class Wall : public EntityModel {
public:
    explicit Wall(const Position& pos);
    ~Wall() override = default;
    
    void update(float deltaTime) override;
    
    /**
     * @brief Get Wall's collision radius
     *
     * Wall collision radius is set to exactly one tile size.
     * A tile is approximately 0.182 world units (2.0 / 11), so the
     * radius is 0.091 (half a tile). This ensures walls fill their
     * entire grid cell and provides consistent collision detection.
     *
     * @return Collision radius = 0.091 (exactly half a tile)
     */
    float getCollisionRadius() const override { return 0.091f; }
};

} // namespace pacman