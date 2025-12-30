#include "representation/views/FruitView.h"
#include "representation/SpriteManager.h"

#include <iostream>

namespace pacman::representation {

FruitView::FruitView(pacman::Fruit& model, std::weak_ptr<pacman::Camera> camera)  // & niet *
    : EntityView(model, camera), fruitModel(model) {
    loadSprite();
}

void FruitView::loadSprite() {
    auto& spriteManager = SpriteManager::getInstance();
    sprite.setTexture(spriteManager.getTexture());

    updateSpriteFromTexture();
}

void FruitView::updateSpriteFromTexture() {
    auto cam = camera.lock();  // ✅ Lock weak_ptr
    if (!cam) return;

    auto& spriteManager = SpriteManager::getInstance();

    try {
        if (spriteManager.hasSpriteRect("fruit")) {
            sf::IntRect rect = spriteManager.getSpriteRect("fruit");
            sprite.setTextureRect(rect);

            sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);

            float targetSize = cam->getSpriteSize();  // ✅ Use locked camera
            float scaleX = targetSize / rect.width;
            float scaleY = targetSize / rect.height;
            sprite.setScale(scaleX, scaleY);
        }
    } catch (const std::exception& e) {
        std::cerr << "FruitView: Error loading sprite: " << e.what() << std::endl;
    }
}

void FruitView::update(float deltaTime) {
    updateSpritePosition();
}

void FruitView::draw(sf::RenderWindow& window) {
    if (!fruitModel.isCollected()) {
        window.draw(sprite);
    }
}

} // namespace pacman::representation