#pragma once
#include "Animation.h"
#include <string>

namespace pacman::representation {

/**
 * @brief Controller voor animation playback
 *
 * === VERANTWOORDELIJKHEDEN ===
 * - Play animation (start vanaf frame 0)
 * - Update over tijd (frame switching op basis van elapsed time)
 * - Provide current sprite naam (voor View rendering)
 *
 * === USAGE IN VIEW ===
 * ```cpp
 * // In View constructor:
 * animationController.play(SpriteManager::getInstance().getAnimation("pacman_walk_right"));
 *
 * // In View::onNotify(ENTITY_UPDATED):
 * animationController.update(event.deltaTime);
 * updateSpriteFromAnimation();  // Haal huidige sprite naam op
 * ```
 *
 * === STATE MACHINE ===
 * playing = false:
 * - Geen updates
 * - getCurrentSpriteName() returns empty
 *
 * playing = true:
 * - Update elapsed time
 * - Switch frame bij elapsed >= frameDuration
 * - Loop of stop op laatste frame
 */
class AnimationController {
public:
    AnimationController() = default;

    /**
     * @brief Start animation playback
     * @param animation Reference naar Animation (moet valid blijven tijdens playback)
     *
     * Reset naar frame 0 en start playing.
     */
    void play(const Animation& animation);

    /**
     * @brief Update animation state
     * @param deltaTime Tijd sinds laatste update (seconden)
     *
     * Roep elke frame aan vanuit View::onNotify(ENTITY_UPDATED).
     *
     * Proces:
     * 1. Add deltaTime to elapsedTime
     * 2. If elapsed >= frameDuration:
     *    - Advance to next frame
     *    - Carry overflow to next frame (smooth animation)
     * 3. If reached end:
     *    - Loop: reset to frame 0
     *    - No loop: stay on last frame, stop playing
     */
    void update(float deltaTime);

    /**
     * @brief Verkrijg huidige sprite naam
     * @return Sprite naam voor huidige frame (empty als niet playing)
     *
     * Gebruikt door View om correct sprite rect te laden:
     * ```cpp
     * std::string spriteName = animationController.getCurrentSpriteName();
     * sf::IntRect rect = SpriteManager::getInstance().getSpriteRect(spriteName);
     * sprite.setTextureRect(rect);
     * ```
     */
    std::string getCurrentSpriteName() const;

    bool isPlaying() const { return playing; }

    /**
     * @brief Check of non-looping animation klaar is
     * @return true als animation finished (voor death animations)
     */
    bool isFinished() const { return !playing && currentAnimation != nullptr; }

    void stop() { playing = false; }

    int getCurrentFrame() const { return currentFrame; }

private:
    const Animation* currentAnimation = nullptr;  // Non-owning pointer (Animation owned door SpriteManager)
    int currentFrame = 0;                          // Huidige frame index (0-based)
    float elapsedTime = 0.0f;                      // Tijd op huidige frame
    bool playing = false;                          // Playback state
};

} // namespace pacman::representation