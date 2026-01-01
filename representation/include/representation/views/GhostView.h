#pragma once
#include "EntityView.h"
#include "logic/entities/Ghost.h"
#include "representation/AnimationController.h"

namespace pacman::representation {

/**
 * @brief View voor Ghost - animaties, scared mode visuals, flickering
 *
 * === ANIMATIES ===
 * Normal mode (per kleur):
 * - "ghost_{color}_walk_{direction}": 2 frames, looping
 * - Colors: red, pink, cyan, orange
 * - Directions: up, down, left, right
 *
 * Scared mode (alle ghosts identiek):
 * - "ghost_scared": blauw sprite, 2 frames
 * - Flickering: wissel tussen frame 1 en 2 op basis van remaining time
 *
 * === FLICKER SYSTEEM ===
 * Doel: Waarschuwing dat scared mode bijna afloopt.
 *
 * Intervals (exponentieel sneller):
 * - > 3.0s: geen flicker (solid blue)
 * - 2-3s: 0.5s interval
 * - 1-2s: 0.25s interval
 * - 0.5-1s: 0.15s interval
 * - < 0.5s: 0.08s interval (zeer snel)
 *
 * === STATE TRACKING ===
 * lastDirection: Detect direction changes → switch animation
 * lastState: Detect ON_MAP ↔ SCARED transitions → switch sprites
 */
class GhostView : public EntityView {
public:
    GhostView(pacman::Ghost& model, std::weak_ptr<pacman::Camera> camera, pacman::GhostColor color);
    ~GhostView() override = default;

    void draw(sf::RenderWindow& window) override;
    void onNotify(const pacman::Event& event) override;

private:
    pacman::Ghost& ghostModel;  // Type-safe access (non-owning)
    pacman::GhostColor ghostColor;
    AnimationController animationController;
    pacman::Direction lastDirection = pacman::Direction::NONE;
    pacman::GhostState lastState = pacman::GhostState::IN_SPAWN;

    // Flicker state voor scared mode warning
    float flickerTimer = 0.0f;
    bool flickerState = false;  // false = frame 1, true = frame 2

    /**
     * @brief Update animation op basis van state en direction
     *
     * Logic:
     * 1. If SCARED: gebruik "ghost_scared" (alle ghosts identiek)
     * 2. Else: gebruik "ghost_{color}_walk_{direction}"
     *
     * Roept animationController.play() aan met nieuwe animation.
     */
    void updateAnimation();

    /**
     * @brief Update sprite vanuit normal animation
     *
     * Gebruikt animationController.getCurrentSpriteName().
     * Voor normal ghost movement.
     */
    void updateSpriteFromAnimation();

    /**
     * @brief Update sprite voor scared mode met flickering
     *
     * Kiest tussen "ghost_scared_1" en "ghost_scared_2" op basis van flickerState.
     * Flicker rate afhankelijk van scaredTimeRemaining.
     */
    void updateSpriteFromScared();
};

} // namespace pacman::representation