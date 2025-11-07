#pragma once
#include "logic/EntityModel.h"

namespace pacman {

class Fruit : public EntityModel {
public:
    explicit Fruit(const Position& pos);
    ~Fruit() override = default;
    
    void update(float deltaTime) override;
    
    bool isCollected() const { return collected; }
    void collect();
    
    int getPointValue() const { return pointValue; }
    
    float getCollisionRadius() const override { return 0.03f; }
    
private:
    bool collected = false;
    int pointValue = 50;
};

} // namespace pacman