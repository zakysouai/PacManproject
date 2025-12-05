// In logic/include/logic/EntityModel.h
#pragma once
#include "logic/utils/Position.h"
#include "logic/utils/Types.h"
#include "logic/patterns/Subject.h"
#include <memory>

namespace pacman {

// ✅ Nieuwe struct voor rechthoekige collision box
struct BoundingBox {
    float left;
    float top;
    float width;
    float height;

    BoundingBox(float l, float t, float w, float h)
        : left(l), top(t), width(w), height(h) {}

    float right() const { return left + width; }
    float bottom() const { return top + height; }

    // Check if two boxes intersect
    bool intersects(const BoundingBox& other) const {
        return !(right() < other.left ||
                 left > other.right() ||
                 bottom() < other.top ||
                 top > other.bottom());
    }
};

class EntityModel : public Subject {
public:
    EntityModel(const Position& pos, float speed = 0.0f)
        : position(pos), speed(speed) {}

    virtual ~EntityModel() = default;

    virtual void update(float deltaTime) = 0;

    // Getters
    Position getPosition() const { return position; }
    float getSpeed() const { return speed; }

    // Setters
    void setPosition(const Position& pos) { position = pos; }
    void setSpeed(float spd) { speed = spd; }

    // ✅ NIEUWE rechthoek-based collision
    virtual BoundingBox getBoundingBox() const {
        // Default: small box centered at position
        float size = getCollisionRadius() * 2.0f;
        return BoundingBox(
            position.x - size/2.0f,
            position.y - size/2.0f,
            size,
            size
        );
    }

    virtual float getCollisionRadius() const { return collisionRadius; }
    void setCollisionRadius(float radius) { collisionRadius = radius; }

    // ✅ Check intersection with another entity
    virtual bool intersects(const EntityModel& other) const {
        return getBoundingBox().intersects(other.getBoundingBox());
    }
    
protected:
    Position position;
    float speed;
    float collisionRadius = 0.05f; // default
};

} // namespace pacman