#include "representation/views/PacManView.h"
#include "representation/SpriteManager.h"
#include <iostream>

namespace pacman::representation {

PacManView::PacManView(pacman::PacMan* model, const pacman::Camera& camera)
    : EntityView(model, camera), pacmanModel(model) {

    // Get the shared sprite sheet texture from SpriteManager
    auto& spriteManager = SpriteManager::getInstance();
    sprite.setTexture(spriteManager.getTexture());

    // Start with a default animation (PacMan facing right, but not moving yet)
    // We'll use "pacman_walk_right" as the default
    // (Later we could add an "idle" animation if we want)
    try {
        animationController.play(spriteManager.getAnimation("pacman_walk_right"));
        std::cout << "PacManView: Started with walk_right animation" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "PacManView: Failed to load default animation: " << e.what() << std::endl;
    }

    // Do initial update to set the first frame
    updateSpriteFromAnimation();
}

void PacManView::update(float deltaTime) {
    // Update sprite position based on model
    updateSpritePosition();

    if (playingDeathAnimation) {
        animationController.update(deltaTime);
        updateSpriteFromAnimation();

        // Check if death animation finished
        if (animationController.isFinished()) {
            playingDeathAnimation = false;
            // Reset to default animation after death
            try {
                animationController.play(SpriteManager::getInstance().getAnimation("pacman_walk_right"));
            } catch (const std::exception& e) {
                std::cerr << "PacManView: Error resetting animation after death: " << e.what() << std::endl;
            }
        }
        return;  // Skip normal animation updates while dying
    }

    // Check if direction changed
    pacman::Direction currentDirection = pacmanModel->getDirection();
    if (currentDirection != lastDirection && currentDirection != pacman::Direction::NONE) {
        switchAnimation(currentDirection);
        lastDirection = currentDirection;
    }

    // Update the animation controller
    animationController.update(deltaTime);

    // Update sprite texture rect based on current animation frame
    updateSpriteFromAnimation();
}

void PacManView::switchAnimation(pacman::Direction direction) {
    auto& spriteManager = SpriteManager::getInstance();

    // Choose animation based on direction
    std::string animationName;
    switch (direction) {
        case pacman::Direction::RIGHT:
            animationName = "pacman_walk_right";
            break;
        case pacman::Direction::LEFT:
            animationName = "pacman_walk_left";
            break;
        case pacman::Direction::UP:
            animationName = "pacman_walk_up";
            break;
        case pacman::Direction::DOWN:
            animationName = "pacman_walk_down";
            break;
        default:
            // No valid direction, don't switch animation
            return;
    }

    // Try to play the new animation
    try {
        if (spriteManager.hasAnimation(animationName)) {
            animationController.play(spriteManager.getAnimation(animationName));
            std::cout << "PacManView: Switched to animation: " << animationName << std::endl;
        } else {
            std::cerr << "PacManView: Animation not found: " << animationName << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "PacManView: Error switching animation: " << e.what() << std::endl;
    }
}

void PacManView::updateSpriteFromAnimation() {
    auto& spriteManager = SpriteManager::getInstance();

    // Get the current sprite name from the animation controller
    std::string spriteName = animationController.getCurrentSpriteName();

    if (spriteName.empty()) {
        return;
    }

    // Look up the sprite rectangle
    try {
        if (spriteManager.hasSpriteRect(spriteName)) {
            sf::IntRect rect = spriteManager.getSpriteRect(spriteName);
            sprite.setTextureRect(rect);

            // ✅ Set origin in TEXTURE coordinates (center of the sprite rect)
            sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);

            // Scale the sprite to the appropriate size for the game
            float targetSize = camera.getSpriteSize();
            float scaleX = targetSize / rect.width;
            float scaleY = targetSize / rect.height;
            sprite.setScale(scaleX, scaleY);
        } else {
            std::cerr << "PacManView: Sprite not found: " << spriteName << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "PacManView: Error updating sprite: " << e.what() << std::endl;
    }
}

void PacManView::draw(sf::RenderWindow& window) {
    if (!pacmanModel) return;

    // Draw the animated sprite
    window.draw(sprite);
}

void PacManView::onNotify(const pacman::Event& event) {
    switch (event.type) {
    case pacman::EventType::DIRECTION_CHANGED:
        // Direction change is already handled in update()
            break;

    case pacman::EventType::PACMAN_DIED:
        std::cout << "PacManView: PacMan died! Playing death animation..." << std::endl;

        // ✅ IMPLEMENT DEATH ANIMATION
        playingDeathAnimation = true;

        try {
            // Play the death animation (non-looping)
            auto& spriteManager = SpriteManager::getInstance();
            if (spriteManager.hasAnimation("pacman_death")) {
                animationController.play(spriteManager.getAnimation("pacman_death"));
                std::cout << "PacManView: Death animation started" << std::endl;
            } else {
                std::cerr << "PacManView: Death animation not found!" << std::endl;
                playingDeathAnimation = false;
            }
        } catch (const std::exception& e) {
            std::cerr << "PacManView: Error playing death animation: " << e.what() << std::endl;
            playingDeathAnimation = false;
        }
        break;

    default:
        break;
    }
}

} // namespace pacman::representation