#include "representation/views/FruitView.h"
#include "representation/SpriteManager.h"

#include <iostream>

namespace pacman::representation {

FruitView::FruitView(pacman::Fruit* model, const pacman::Camera& camera)
    : EntityView(model, camera), fruitModel(model) {
    loadSprite();
}

void FruitView::loadSprite() {
    // Get the shared sprite sheet texture from SpriteManager
    auto& spriteManager = SpriteManager::getInstance();
    sprite.setTexture(spriteManager.getTexture());

    // Load the fruit sprite
    updateSpriteFromTexture();
}

void FruitView::updateSpriteFromTexture() {
    auto& spriteManager = SpriteManager::getInstance();

    try {
        if (spriteManager.hasSpriteRect("fruit")) {
            sf::IntRect rect = spriteManager.getSpriteRect("fruit");
            sprite.setTextureRect(rect);

            // ✅ Set origin in texture coordinates (center of sprite)
            sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);

            // Scale sprite appropriately
            float targetSize = camera.getSpriteSize();
            float scaleX = targetSize / rect.width;
            float scaleY = targetSize / rect.height;
            sprite.setScale(scaleX, scaleY);
        }
    } catch (const std::exception& e) {
        std::cerr << "FruitView: Error loading sprite: " << e.what() << std::endl;
    }
}

void FruitView::update(float deltaTime) {
    // Update sprite position
    updateSpritePosition();
}

void FruitView::draw(sf::RenderWindow& window) {
    // Only draw if not collected
    if (!fruitModel->isCollected()) {
        window.draw(sprite);
    }
}

} // namespace pacman::representation