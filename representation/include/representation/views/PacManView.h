#pragma once
#include "EntityView.h"
#include "logic/entities/PacMan.h"
#include "representation/AnimationController.h"

namespace pacman::representation {

/**
 * @brief View voor PacMan - animaties en rendering
 *
 * === ANIMATIES ===
 * Movement animations (looping):
 * - "pacman_walk_right": right_closed → half → open → half
 * - "pacman_walk_left": left_closed → half → open → half
 * - "pacman_walk_up": up_closed → half → open → half
 * - "pacman_walk_down": down_closed → half → open → half
 *
 * Death animation (non-looping):
 * - "pacman_death": 11 frames, blijft op laatste frame
 *
 * === EVENT HANDLING ===
 * - ENTITY_UPDATED: Update position, advance animation
 * - DIRECTION_CHANGED: Switch naar nieuwe direction animation
 * - PACMAN_DIED: Play death animation
 */
class PacManView : public EntityView {
public:
    PacManView(pacman::PacMan& model, std::weak_ptr<pacman::Camera> camera);
    ~PacManView() override = default;

    void draw(sf::RenderWindow& window) override;
    void onNotify(const pacman::Event& event) override;

private:
    pacman::PacMan* pacmanModel;  // Raw pointer voor type-safe access (non-owning)
    AnimationController animationController;
    pacman::Direction lastDirection = pacman::Direction::NONE;
    bool playingDeathAnimation = false;

    /**
     * @brief Switch animation op basis van direction
     * @param direction Nieuwe movement direction
     *
     * Laadt juiste animation vanuit SpriteManager.
     * Roept animationController.play() aan.
     */
    void switchAnimation(pacman::Direction direction);

    /**
     * @brief Update sprite op basis van huidige animation frame
     *
     * Proces:
     * 1. Haal sprite naam op van animationController
     * 2. Lookup sprite rect in SpriteManager
     * 3. Update sprite texture rect
     * 4. Scale sprite op basis van Camera::getSpriteSize()
     */
    void updateSpriteFromAnimation();
};

} // namespace pacman::representation