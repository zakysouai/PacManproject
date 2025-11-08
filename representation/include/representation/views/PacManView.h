#pragma once
#include "EntityView.h"
#include "logic/entities/PacMan.h"
#include "representation/AnimationController.h"

namespace pacman::representation {

class PacManView : public EntityView {
public:
    PacManView(pacman::PacMan* model, const pacman::Camera& camera);
    ~PacManView() override = default;

    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void onNotify(const pacman::Event& event) override;

private:
    pacman::PacMan* pacmanModel;

    // Animation system
    AnimationController animationController;
    pacman::Direction lastDirection = pacman::Direction::NONE;

    /**
     * @brief Switch to the appropriate animation based on PacMan's direction
     *
     * @param direction The direction PacMan is moving
     */
    void switchAnimation(pacman::Direction direction);

    /**
     * @brief Update the sprite's texture rectangle based on current animation frame
     */
    void updateSpriteFromAnimation();
};

} // namespace pacman::representation