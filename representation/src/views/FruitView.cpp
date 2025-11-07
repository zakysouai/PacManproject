#include "representation/views/FruitView.h"

namespace pacman::representation {

FruitView::FruitView(pacman::Fruit* model, const pacman::Camera& camera)
    : EntityView(model, camera), fruitModel(model) {
    loadTexture();
}

void FruitView::loadTexture() {
    // TODO: Load sprite sheet
    // texture.loadFromFile("resources/sprites/fruit.png");
    // sprite.setTexture(texture);
}

void FruitView::draw(sf::RenderWindow& window) {
    if (!fruitModel || fruitModel->isCollected()) return;
    
    auto worldPos = fruitModel->getPosition();
    auto screenPos = camera.worldToScreen(worldPos);
    
    // Draw as a simple shape for now
    float size = camera.getSpriteSize() * 0.5f;
    sf::RectangleShape rect(sf::Vector2f(size, size));
    rect.setFillColor(sf::Color(255, 140, 0));  // Orange
    rect.setOrigin(size / 2, size / 2);
    rect.setPosition(screenPos.x, screenPos.y);
    
    window.draw(rect);
}

} // namespace pacman::representation