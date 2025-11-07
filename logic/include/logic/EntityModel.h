#pragma once
#include "logic/utils/Position.h"
#include "logic/utils/Types.h"
#include "logic/patterns/Subject.h"
#include <memory>

namespace pacman {

class EntityModel : public Subject {
public:
    EntityModel(const Position& pos, float speed = 0.0f)
        : position(pos), speed(speed) {}
    
    virtual ~EntityModel() = default;
    
    // Pure virtual update method
    virtual void update(float deltaTime) = 0;
    
    // Getters
    Position getPosition() const { return position; }
    float getSpeed() const { return speed; }
    
    // Setters
    void setPosition(const Position& pos) { position = pos; }
    void setSpeed(float spd) { speed = spd; }
    
    // Collision detection (simple AABB)
    virtual bool intersects(const EntityModel& other) const {
        float distance = position.distance(other.position);
        return distance < getCollisionRadius() + other.getCollisionRadius();
    }
    
    virtual float getCollisionRadius() const { return 0.05f; }
    
protected:
    Position position;
    float speed;
};

} // namespace pacman