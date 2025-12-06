#include "representation/views/GhostView.h"
#include "representation/SpriteManager.h"
#include <iostream>

namespace pacman::representation {

GhostView::GhostView(pacman::Ghost* model, const pacman::Camera& camera, pacman::GhostColor color)
    : EntityView(model, camera), ghostModel(model), ghostColor(color) {

    auto& spriteManager = SpriteManager::getInstance();
    sprite.setTexture(spriteManager.getTexture());

    // ✅ Initialize tracking
    lastDirection = ghostModel->getCurrentDirection();
    lastState = ghostModel->getState();

    updateAnimation();
}

void GhostView::update(float deltaTime) {
    updateSpritePosition();

    // ✅ Check if direction or state changed
    pacman::Direction currentDir = ghostModel->getCurrentDirection();
    pacman::GhostState currentState = ghostModel->getState();

    if (currentDir != lastDirection || currentState != lastState) {
        updateAnimation();
        lastDirection = currentDir;
        lastState = currentState;
    }

    animationController.update(deltaTime);
    updateSpriteFromAnimation();
}

void GhostView::updateAnimation() {
    auto& spriteManager = SpriteManager::getInstance();
    std::string animationName;

    // ✅ Check if scared
    if (ghostModel->isScared()) {
        animationName = "ghost_scared";
    } else {
        // Normal color-based animation
        std::string colorPrefix;
        switch (ghostColor) {
            case pacman::GhostColor::RED:    colorPrefix = "ghost_red"; break;
            case pacman::GhostColor::PINK:   colorPrefix = "ghost_pink"; break;
            case pacman::GhostColor::BLUE:   colorPrefix = "ghost_cyan"; break;
            case pacman::GhostColor::ORANGE: colorPrefix = "ghost_orange"; break;
        }

        // Direction-based
        pacman::Direction dir = ghostModel->getCurrentDirection();
        std::string dirSuffix;
        switch (dir) {
            case pacman::Direction::UP:    dirSuffix = "_walk_up"; break;
            case pacman::Direction::DOWN:  dirSuffix = "_walk_down"; break;
            case pacman::Direction::LEFT:  dirSuffix = "_walk_left"; break;
            case pacman::Direction::RIGHT: dirSuffix = "_walk_right"; break;
            default: dirSuffix = "_walk_right"; break;
        }

        animationName = colorPrefix + dirSuffix;
    }

    try {
        if (spriteManager.hasAnimation(animationName)) {
            animationController.play(spriteManager.getAnimation(animationName));
        }
    } catch (const std::exception& e) {
        std::cerr << "GhostView: Error loading animation: " << e.what() << std::endl;
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
            sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);

            float targetSize = camera.getSpriteSize();
            float scaleX = targetSize / rect.width;
            float scaleY = targetSize / rect.height;
            sprite.setScale(scaleX, scaleY);
        }
    } catch (const std::exception& e) {
        std::cerr << "GhostView: Error updating sprite: " << e.what() << std::endl;
    }
}

void GhostView::onNotify(const pacman::Event& event) {
    if (event.type == pacman::EventType::GHOST_STATE_CHANGED) {
        updateAnimation();
    }
}

void GhostView::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

} // namespace pacman::representation