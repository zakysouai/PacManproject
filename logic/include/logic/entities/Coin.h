#pragma once
#include "logic/EntityModel.h"

namespace pacman {

class Coin : public EntityModel {
public:
    explicit Coin(const Position& pos);
    ~Coin() override = default;
    
    void update(float deltaTime) override;
    
    bool isCollected() const { return collected; }
    void collect();
    
    int getPointValue() const { return pointValue; }
    
    // float getCollisionRadius() const override { return 0.02f; }
    
private:
    bool collected = false;
    int pointValue = 10;
};

} // namespace pacman