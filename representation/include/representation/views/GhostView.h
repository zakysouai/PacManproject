#pragma once
#include "EntityView.h"
#include "logic/entities/Ghost.h"
#include "representation/AnimationController.h"

namespace pacman::representation {

class GhostView : public EntityView {
public:
    GhostView(pacman::Ghost& model, std::weak_ptr<pacman::Camera> camera, pacman::GhostColor color);
    ~GhostView() override = default;

    void draw(sf::RenderWindow& window) override;
    void onNotify(const pacman::Event& event) override;

private:
    pacman::Ghost* ghostModel;
    pacman::GhostColor ghostColor;
    AnimationController animationController;
    pacman::Direction lastDirection = pacman::Direction::NONE;
    pacman::GhostState lastState = pacman::GhostState::IN_SPAWN;

    float flickerTimer = 0.0f;
    bool flickerState = false;

    void updateAnimation();
    void updateSpriteFromAnimation();
    void updateSpriteFromScared();
};

} // namespace pacman::representation