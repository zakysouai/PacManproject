#pragma once
#include "logic/EntityModel.h"

namespace pacman {

enum class GhostColor {
    RED,
    PINK,
    BLUE,
    ORANGE
};

class Ghost : public EntityModel {
public:
    explicit Ghost(const Position& pos, GhostColor color);
    ~Ghost() override = default;

    void update(float deltaTime) override;

    GhostColor getColor() const { return color; }
    Direction getCurrentDirection() const { return currentDirection; }

    void setWorld(class World* world) { this->world = world; }
    float getCollisionRadius() const override { return 0.08f; }

private:
    GhostColor color;
    Direction currentDirection = Direction::RIGHT;
    World* world = nullptr;

    void move(float deltaTime);
    bool isAtIntersection() const;
    Direction chooseDirectionAtIntersection();
    void handleWallCollision();
    bool isOpposite(Direction dir1, Direction dir2) const;
};

} // namespace pacman