#include "representation/AnimationController.h"

namespace pacman::representation {

void AnimationController::play(const Animation& animation) {
    // Safety check: make sure the animation is valid
    if (!animation.isValid()) {
        playing = false;
        return;
    }
    
    // Set up the controller to play this animation
    currentAnimation = &animation;
    currentFrame = 0;           // Start at first frame
    elapsedTime = 0.0f;         // Reset timer
    playing = true;             // Start playing
}

void AnimationController::update(float deltaTime) {
    // Don't do anything if we're not playing
    if (!playing || currentAnimation == nullptr) {
        return;
    }
    
    // Safety check: make sure animation is still valid
    if (!currentAnimation->isValid()) {
        playing = false;
        return;
    }
    
    // Add time to our timer
    elapsedTime += deltaTime;
    
    // Check if it's time to move to the next frame
    if (elapsedTime >= currentAnimation->frameDuration) {
        // Move time overflow to next frame (important for smooth animation!)
        elapsedTime -= currentAnimation->frameDuration;
        
        // Move to next frame
        currentFrame++;
        
        // Check if we've reached the end of the animation
        if (currentFrame >= static_cast<int>(currentAnimation->frameNames.size())) {
            if (currentAnimation->loop) {
                // Loop back to the beginning
                currentFrame = 0;
            } else {
                // Non-looping animation: stay on last frame and stop playing
                currentFrame = currentAnimation->frameNames.size() - 1;
                playing = false;
            }
        }
    }
}

std::string AnimationController::getCurrentSpriteName() const {
    // Safety checks
    if (currentAnimation == nullptr || !currentAnimation->isValid()) {
        return "";  // Return empty string if no valid animation
    }
    
    // Make sure currentFrame is within bounds
    if (currentFrame < 0 || currentFrame >= static_cast<int>(currentAnimation->frameNames.size())) {
        return "";
    }
    
    // Return the sprite name for the current frame
    return currentAnimation->frameNames[currentFrame];
}

} // namespace pacman::representation