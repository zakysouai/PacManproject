// representation/include/representation/views/GhostView.h
#pragma once
#include "EntityView.h"
#include "logic/entities/Ghost.h"
#include "representation/AnimationController.h"

namespace pacman::representation {

class GhostView : public EntityView {
public:
    GhostView(pacman::Ghost* model, const pacman::Camera& camera, pacman::GhostColor color);
    ~GhostView() override = default;

    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;

private:
    pacman::Ghost* ghostModel;
    pacman::GhostColor ghostColor;

    // Animation system
    AnimationController animationController;
    pacman::Direction lastDirection = pacman::Direction::NONE;

    void switchAnimation(pacman::Direction direction);
    void updateSpriteFromAnimation();
    std::string getColorPrefix() const;
};

} // namespace pacman::representation