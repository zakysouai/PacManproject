#pragma once
#include <vector>
#include <string>

namespace pacman::representation {

/**
 * @brief Animation definitie - sequence van sprite frames
 *
 * === CONCEPT ===
 * Animation = lijst van sprite namen + timing info
 *
 * Voorbeeld:
 * ```cpp
 * Animation pacmanWalk {
 *     .frameNames = {"pacman_right_closed", "pacman_right_half", "pacman_right_open", "pacman_right_half"},
 *     .frameDuration = 0.1f,  // 10 FPS
 *     .loop = true
 * };
 * ```
 *
 * === TYPES ===
 * - Looping: Movement animaties (pacman walk, ghost walk)
 * - Non-looping: One-shot animaties (pacman death)
 *
 * === STORAGE ===
 * Animations worden gedefinieerd in SpriteManager::defineAllAnimations()
 * en opgehaald via SpriteManager::getAnimation(name).
 */
struct Animation {
    /**
     * @brief Frame sequence (sprite namen in volgorde)
     *
     * Elke string komt overeen met sprite naam in SpriteManager.
     * Animatie cyclet door deze frames op basis van frameDuration.
     */
    std::vector<std::string> frameNames;

    /**
     * @brief Duur van elk frame in seconden
     *
     * Hoe lang elk frame getoond wordt voordat naar volgende.
     * Voorbeeld: 0.1f = 10 frames per seconde
     */
    float frameDuration;

    /**
     * @brief Loop gedrag
     *
     * - true: Na laatste frame terug naar frame 0 (walk animaties)
     * - false: Na laatste frame blijf op laatste frame (death animatie)
     */
    bool loop;

    // Constructors
    Animation() : frameDuration(0.1f), loop(true) {}

    Animation(const std::vector<std::string>& frames, float duration, bool shouldLoop)
        : frameNames(frames), frameDuration(duration), loop(shouldLoop) {}

    /**
     * @brief Validatie check
     * @return true als animation minimaal 1 frame heeft en duration > 0
     */
    bool isValid() const {
        return !frameNames.empty() && frameDuration > 0.0f;
    }

    /**
     * @brief Verkrijg aantal frames
     */
    size_t getFrameCount() const {
        return frameNames.size();
    }
};

} // namespace pacman::representation