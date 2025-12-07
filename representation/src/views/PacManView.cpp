#include "representation/views/PacManView.h"
#include "representation/SpriteManager.h"
#include <iostream>

namespace pacman::representation {

PacManView::PacManView(pacman::PacMan* model, const pacman::Camera& camera)
    : EntityView(model, camera), pacmanModel(model) {

    auto& spriteManager = SpriteManager::getInstance();
    sprite.setTexture(spriteManager.getTexture());

    try {
        animationController.play(spriteManager.getAnimation("pacman_walk_right"));
        std::cout << "PacManView: Started with walk_right animation" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "PacManView: Failed to load default animation: " << e.what() << std::endl;
    }

    updateSpriteFromAnimation();
}

void PacManView::onNotify(const pacman::Event& event) {
    switch (event.type) {
    case pacman::EventType::ENTITY_UPDATED: {  // ✅ BRACES
        updateSpritePosition();

        if (!playingDeathAnimation) {
            pacman::Direction currentDirection = pacmanModel->getDirection();
            if (currentDirection != lastDirection && currentDirection != pacman::Direction::NONE) {
                switchAnimation(currentDirection);
                lastDirection = currentDirection;
            }

            animationController.update(event.deltaTime);
            updateSpriteFromAnimation();
        } else {
            animationController.update(event.deltaTime);
            updateSpriteFromAnimation();

            if (animationController.isFinished()) {
                playingDeathAnimation = false;
                try {
                    animationController.play(SpriteManager::getInstance().getAnimation("pacman_walk_right"));
                } catch (const std::exception& e) {
                    std::cerr << "PacManView: Error resetting animation: " << e.what() << std::endl;
                }
            }
        }
        break;
    }  // ✅ BRACES

    case pacman::EventType::DIRECTION_CHANGED:
        break;

    case pacman::EventType::PACMAN_DIED: {  // ✅ BRACES (voor consistency)
        std::cout << "PacManView: PacMan died! Playing death animation..." << std::endl;
        playingDeathAnimation = true;

        try {
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
    }  // ✅ BRACES

    default:
        break;
    }
}

void PacManView::switchAnimation(pacman::Direction direction) {
    auto& spriteManager = SpriteManager::getInstance();

    std::string animationName;
    switch (direction) {
        case pacman::Direction::RIGHT: animationName = "pacman_walk_right"; break;
        case pacman::Direction::LEFT:  animationName = "pacman_walk_left";  break;
        case pacman::Direction::UP:    animationName = "pacman_walk_up";    break;
        case pacman::Direction::DOWN:  animationName = "pacman_walk_down";  break;
        default: return;
    }

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
        } else {
            std::cerr << "PacManView: Sprite not found: " << spriteName << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "PacManView: Error updating sprite: " << e.what() << std::endl;
    }
}

void PacManView::draw(sf::RenderWindow& window) {
    if (!pacmanModel) return;
    window.draw(sprite);
}

} // namespace pacman::representation