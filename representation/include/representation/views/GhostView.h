#pragma once
#include "EntityView.h"
#include "logic/entities/Ghost.h"
#include "representation/AnimationController.h"

namespace pacman::representation {

class GhostView : public EntityView {
public:
    GhostView(pacman::Ghost* model, const pacman::Camera& camera);
    ~GhostView() override = default;

    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void onNotify(const pacman::Event& event) override;

private:
    pacman::Ghost* ghostModel;

    // Animation system
    AnimationController animationController;
    pacman::Direction lastDirection = pacman::Direction::RIGHT;
    bool lastFearState = false;

    /**
     * @brief Update animation based on ghost's current state and direction
     */
    void updateAnimation();

    /**
     * @brief Get the animation name for the current ghost state
     *
     * @return Animation name (e.g., "ghost_red_walk_up" or "ghost_scared")
     */
    std::string getAnimationName() const;

    /**
     * @brief Update sprite texture rect from current animation frame
     */
    void updateSpriteFromAnimation();
};

} // namespace pacman::representation