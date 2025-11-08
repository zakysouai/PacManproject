#pragma once
#include <vector>
#include <string>

namespace pacman::representation {

/**
 * @brief Animation definition - describes a sequence of sprite frames
 * 
 * An Animation is simply a list of sprite names that should be displayed
 * in sequence, along with timing information.
 * 
 * Example:
 *   Animation pacmanWalk {
 *       .frameNames = {"pacman_right_closed", "pacman_right_open"},
 *       .frameDuration = 0.1f,
 *       .loop = true
 *   };
 */
struct Animation {
    /**
     * @brief List of sprite names that make up this animation
     * 
     * Each string should correspond to a sprite name registered in SpriteManager.
     * The animation will cycle through these frames in order.
     * 
     * Example: {"pacman_r_0", "pacman_r_1", "pacman_r_2", "pacman_r_1"}
     */
    std::vector<std::string> frameNames;
    
    /**
     * @brief Duration of each frame in seconds
     * 
     * How long each frame should be displayed before moving to the next.
     * Example: 0.1f means 10 frames per second
     */
    float frameDuration;
    
    /**
     * @brief Whether the animation should loop
     * 
     * - true: After the last frame, go back to frame 0 (walking animations)
     * - false: After the last frame, stay on last frame (death animations)
     */
    bool loop;
    
    /**
     * @brief Default constructor - creates empty animation
     */
    Animation() : frameDuration(0.1f), loop(true) {}
    
    /**
     * @brief Constructor with parameters
     */
    Animation(const std::vector<std::string>& frames, float duration, bool shouldLoop)
        : frameNames(frames), frameDuration(duration), loop(shouldLoop) {}
    
    /**
     * @brief Check if animation is valid
     * @return true if animation has at least one frame
     */
    bool isValid() const {
        return !frameNames.empty() && frameDuration > 0.0f;
    }
    
    /**
     * @brief Get number of frames in this animation
     */
    size_t getFrameCount() const {
        return frameNames.size();
    }
};

} // namespace pacman::representation