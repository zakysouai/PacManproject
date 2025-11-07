#pragma once
#include "logic/EntityModel.h"

namespace pacman {

class Wall : public EntityModel {
public:
    explicit Wall(const Position& pos);
    ~Wall() override = default;
    
    void update(float deltaTime) override;
    
    float getCollisionRadius() const override { return 0.05f; }
};

} // namespace pacman