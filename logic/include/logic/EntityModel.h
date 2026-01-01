#pragma once
#include "logic/utils/Position.h"
#include "logic/utils/Types.h"
#include "logic/patterns/Subject.h"
#include <memory>

namespace pacman {

/**
 * @brief Rechthoekige collision box voor nauwkeurige botsingsdetectie
 *
 * Gebruikt axis-aligned bounding boxes (AABB) voor efficiënte collision checks.
 * Voorkomt problemen met circulaire collision (entities vastlopen in hoeken).
 */
struct BoundingBox {
    float left;
    float top;
    float width;
    float height;

    BoundingBox(float l, float t, float w, float h)
        : left(l), top(t), width(w), height(h) {}

    float right() const { return left + width; }
    float bottom() const { return top + height; }

    /**
     * @brief Check of twee boxes elkaar overlappen
     * @return true als er intersectie is
     */
    bool intersects(const BoundingBox& other) const {
        return !(right() < other.left ||
                 left > other.right() ||
                 bottom() < other.top ||
                 top > other.bottom());
    }
};

/**
 * @brief Abstracte basisklasse voor alle game entities
 *
 * Bevat gemeenschappelijke functionaliteit:
 * - Positie in genormaliseerde coördinaten [-1, 1]
 * - Snelheid voor movement
 * - Collision detection via bounding boxes
 * - Observer pattern (Subject) voor view updates
 *
 * Subklassen moeten update() implementeren voor entity-specifieke logica.
 */
class EntityModel : public Subject {
public:
    EntityModel(const Position& pos, float speed = 0.0f)
        : position(pos), speed(speed) {}

    virtual ~EntityModel() = default;

    /**
     * @brief Update entity state (pure virtual)
     * @param deltaTime Tijd sinds laatste frame in seconden
     *
     * Moet geïmplementeerd worden door subklassen.
     * Typisch: movement, AI, state changes, notify observers.
     */
    virtual void update(float deltaTime) = 0;

    // Getters
    Position getPosition() const { return position; }
    float getSpeed() const { return speed; }

    // Setters
    void setPosition(const Position& pos) { position = pos; }
    void setSpeed(float spd) { speed = spd; }

    /**
     * @brief Verkrijg collision box voor deze entity
     * @return BoundingBox gecentreerd op positie
     *
     * Default: vierkante box met grootte 2*collisionRadius.
     * Kan geoverride worden voor custom collision shapes.
     */
    virtual BoundingBox getBoundingBox() const {
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

    /**
     * @brief Check of deze entity botst met een andere
     * @param other De andere entity
     * @return true bij collision
     */
    virtual bool intersects(const EntityModel& other) const {
        return getBoundingBox().intersects(other.getBoundingBox());
    }

protected:
    Position position;
    float speed;
    float collisionRadius = 0.05f;  // Default waarde, wordt overschreven in World::parseMap
};

} // namespace pacman