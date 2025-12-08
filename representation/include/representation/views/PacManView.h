#pragma once
#include "EntityView.h"
#include "logic/entities/PacMan.h"
#include "representation/AnimationController.h"

namespace pacman::representation {

class PacManView : public EntityView {
public:
    PacManView(pacman::PacMan& model, std::weak_ptr<pacman::Camera> camera);  // ✅ CHANGED
    ~PacManView() override = default;

    void draw(sf::RenderWindow& window) override;
    void onNotify(const pacman::Event& event) override;

private:
    pacman::PacMan* pacmanModel;
    AnimationController animationController;
    pacman::Direction lastDirection = pacman::Direction::NONE;
    bool playingDeathAnimation = false;

    void switchAnimation(pacman::Direction direction);
    void updateSpriteFromAnimation();
};

} // namespace pacman::representation