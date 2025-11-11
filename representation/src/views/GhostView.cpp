#include "representation/views/GhostView.h"
#include "representation/SpriteManager.h"
#include <iostream>

namespace pacman::representation {

GhostView::GhostView(pacman::Ghost* model, const pacman::Camera& camera)
    : EntityView(model, camera), ghostModel(model) {

    // ✅ Assign unique color based on creation order
    static int ghostCounter = 0;
    static const std::vector<std::string> colors = {"red", "pink", "cyan", "orange"};
    ghostColor = colors[ghostCounter % colors.size()];
    ghostCounter++;

    std::cout << "GhostView: Created with color " << ghostColor << std::endl;

    // Get the shared sprite sheet texture
    auto& spriteManager = SpriteManager::getInstance();
    sprite.setTexture(spriteManager.getTexture());

    // Start with default animation
    try {
        std::string initialAnim = getAnimationName();
        if (spriteManager.hasAnimation(initialAnim)) {
            animationController.play(spriteManager.getAnimation(initialAnim));
            std::cout << "GhostView: Started with animation: " << initialAnim << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "GhostView: Failed to load initial animation: " << e.what() << std::endl;
    }

    // Initial update
    updateSpriteFromAnimation();
}

void GhostView::update(float deltaTime) {
    // Update sprite position
    updateSpritePosition();

    // ✅ Get current direction from ghost model
    pacman::Direction currentDirection = ghostModel->getCurrentDirection();

    // Check if ghost state or direction changed
    bool currentFearState = ghostModel->isFeared();

    // If state changed, switch animation
    if (currentFearState != lastFearState || currentDirection != lastDirection) {
        std::string animationName = getAnimationName();

        auto& spriteManager = SpriteManager::getInstance();
        try {
            if (spriteManager.hasAnimation(animationName)) {
                animationController.play(spriteManager.getAnimation(animationName));
                std::cout << "GhostView: Switched to animation: " << animationName << std::endl;
            } else {
                std::cerr << "GhostView: Animation not found: " << animationName << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "GhostView: Error switching animation: " << e.what() << std::endl;
        }

        lastFearState = currentFearState;
        lastDirection = currentDirection;
    }

    // Update animation controller
    animationController.update(deltaTime);

    // Update sprite texture rect
    updateSpriteFromAnimation();
}

std::string GhostView::getAnimationName() const {
    // If in fear mode, use scared animation (same for all ghosts)
    if (ghostModel->isFeared()) {
        return "ghost_scared";
    }

    // ✅ Get direction from ghost model
    pacman::Direction currentDirection = ghostModel->getCurrentDirection();

    // Get direction suffix
    std::string directionSuffix = "right";  // Default
    switch (currentDirection) {
        case pacman::Direction::UP:    directionSuffix = "up"; break;
        case pacman::Direction::DOWN:  directionSuffix = "down"; break;
        case pacman::Direction::LEFT:  directionSuffix = "left"; break;
        case pacman::Direction::RIGHT: directionSuffix = "right"; break;
        default: directionSuffix = "right"; break;
    }

    // Construct animation name: "ghost_red_walk_up"
    return "ghost_" + ghostColor + "_walk_" + directionSuffix;
}

void GhostView::updateSpriteFromAnimation() {
    auto& spriteManager = SpriteManager::getInstance();

    // Get current sprite name from animation
    std::string spriteName = animationController.getCurrentSpriteName();

    if (spriteName.empty()) {
        return;
    }

    // Look up and apply sprite rectangle
    try {
        if (spriteManager.hasSpriteRect(spriteName)) {
            sf::IntRect rect = spriteManager.getSpriteRect(spriteName);
            sprite.setTextureRect(rect);

            // Set origin in texture coordinates (center of sprite)
            sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);

            // Scale sprite appropriately
            float targetSize = camera.getSpriteSize();
            float scaleX = targetSize / rect.width;
            float scaleY = targetSize / rect.height;
            sprite.setScale(scaleX, scaleY);
        } else {
            std::cerr << "GhostView: Sprite not found: " << spriteName << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "GhostView: Error updating sprite: " << e.what() << std::endl;
    }
}

void GhostView::draw(sf::RenderWindow& window) {
    if (!ghostModel) return;

    // Draw the animated sprite
    window.draw(sprite);
}

void GhostView::onNotify(const pacman::Event& event) {
    // Handle ghost-specific events
    EntityView::onNotify(event);
}

} // namespace pacman::representation