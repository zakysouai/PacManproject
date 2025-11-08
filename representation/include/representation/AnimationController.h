#pragma once
#include "Animation.h"
#include <string>

namespace pacman::representation {

/**
 * @brief Controls the playback of an Animation
 * 
 * The AnimationController is responsible for:
 * - Playing an animation (tracking which frame we're on)
 * - Updating the animation over time (switching frames)
 * - Providing the current sprite name to display
 * 
 * Usage:
 *   AnimationController controller;
 *   controller.play(someAnimation);
 *   
 *   // In update loop:
 *   controller.update(deltaTime);
 *   std::string sprite = controller.getCurrentSpriteName();
 */
class AnimationController {
public:
    /**
     * @brief Default constructor - creates a controller with no animation
     */
    AnimationController() = default;
    
    /**
     * @brief Start playing an animation
     * 
     * This resets the controller to frame 0 and starts playing from the beginning.
     * 
     * @param animation The animation to play
     */
    void play(const Animation& animation);
    
    /**
     * @brief Update the animation (call every frame)
     * 
     * This advances the animation timer and switches to the next frame when needed.
     * 
     * @param deltaTime Time elapsed since last update (in seconds)
     */
    void update(float deltaTime);
    
    /**
     * @brief Get the name of the sprite that should currently be displayed
     * 
     * @return Sprite name (e.g., "pacman_right_open")
     * @return Empty string if no animation is playing or animation is invalid
     */
    std::string getCurrentSpriteName() const;
    
    /**
     * @brief Check if the animation is currently playing
     * 
     * @return true if an animation is active and playing
     */
    bool isPlaying() const { return playing; }
    
    /**
     * @brief Check if a non-looping animation has finished
     * 
     * This is useful for death animations or other one-shot animations.
     * 
     * @return true if animation is done (only for non-looping animations)
     */
    bool isFinished() const { return !playing && currentAnimation != nullptr; }
    
    /**
     * @brief Stop the current animation
     */
    void stop() { playing = false; }
    
    /**
     * @brief Get the current frame index
     * 
     * @return Current frame (0-based index)
     */
    int getCurrentFrame() const { return currentFrame; }

private:
    const Animation* currentAnimation = nullptr;  // Pointer to the animation we're playing
    int currentFrame = 0;                          // Which frame are we currently showing?
    float elapsedTime = 0.0f;                      // Time spent on current frame
    bool playing = false;                          // Is the animation active?
};

} // namespace pacman::representation