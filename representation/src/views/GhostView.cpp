// representation/src/views/GhostView.cpp
#include "representation/views/GhostView.h"
#include "representation/SpriteManager.h"
#include <iostream>

namespace pacman::representation {

GhostView::GhostView(pacman::Ghost* model, const pacman::Camera& camera, pacman::GhostColor color)
    : EntityView(model, camera), ghostModel(model), ghostColor(color) {

    auto& spriteManager = SpriteManager::getInstance();
    sprite.setTexture(spriteManager.getTexture());

    // Start met default animation (rechts)
    try {
        std::string animName = getColorPrefix() + "_walk_right";
        animationController.play(spriteManager.getAnimation(animName));
        std::cout << "GhostView: Started with " << animName << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "GhostView: Failed to load animation: " << e.what() << std::endl;
    }

    updateSpriteFromAnimation();
}

std::string GhostView::getColorPrefix() const {
    switch (ghostColor) {
    case pacman::GhostColor::RED:    return "ghost_red";
    case pacman::GhostColor::PINK:   return "ghost_pink";
    case pacman::GhostColor::BLUE:   return "ghost_cyan";
    case pacman::GhostColor::ORANGE: return "ghost_orange";
    default: return "ghost_red";
    }
}

void GhostView::update(float deltaTime) {
    updateSpritePosition();

    // Check direction change
    pacman::Direction currentDirection = ghostModel->getCurrentDirection();
    if (currentDirection != lastDirection && currentDirection != pacman::Direction::NONE) {
        switchAnimation(currentDirection);
        lastDirection = currentDirection;
    }

    animationController.update(deltaTime);
    updateSpriteFromAnimation();
}

void GhostView::switchAnimation(pacman::Direction direction) {
    auto& spriteManager = SpriteManager::getInstance();

    std::string animationName = getColorPrefix();

    switch (direction) {
    case pacman::Direction::RIGHT:
        animationName += "_walk_right";
        break;
    case pacman::Direction::LEFT:
        animationName += "_walk_left";
        break;
    case pacman::Direction::UP:
        animationName += "_walk_up";
        break;
    case pacman::Direction::DOWN:
        animationName += "_walk_down";
        break;
    default:
        return;
    }

    try {
        if (spriteManager.hasAnimation(animationName)) {
            animationController.play(spriteManager.getAnimation(animationName));
            std::cout << "GhostView: Switched to " << animationName << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "GhostView: Error switching animation: " << e.what() << std::endl;
    }
}

void GhostView::updateSpriteFromAnimation() {
    auto& spriteManager = SpriteManager::getInstance();

    std::string spriteName = animationController.getCurrentSpriteName();
    if (spriteName.empty()) return;

    try {
        if (spriteManager.hasSpriteRect(spriteName)) {
            sf::IntRect rect = spriteManager.getSpriteRect(spriteName);
            sprite.setTextureRect(rect);

            // Origin in texture coordinates
            sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);

            // Scale
            float targetSize = camera.getSpriteSize();
            float scaleX = targetSize / rect.width;
            float scaleY = targetSize / rect.height;
            sprite.setScale(scaleX, scaleY);
        }
    } catch (const std::exception& e) {
        std::cerr << "GhostView: Error updating sprite: " << e.what() << std::endl;
    }
}

void GhostView::draw(sf::RenderWindow& window) {
    if (!ghostModel) return;
    window.draw(sprite);
}

} // namespace pacman::representation