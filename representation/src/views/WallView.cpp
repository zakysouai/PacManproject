#include "representation/views/WallView.h"

namespace pacman::representation {

WallView::WallView(pacman::Wall* model, const pacman::Camera& camera)
    : EntityView(model, camera), wallModel(model) {
    setupRectangle();
}

void WallView::setupRectangle() {
    float size = camera.getSpriteSize();
    rectangle.setSize(sf::Vector2f(size, size));
    rectangle.setFillColor(sf::Color::Blue);
    rectangle.setOrigin(size / 2, size / 2);
}

void WallView::draw(sf::RenderWindow& window) {
    auto worldPos = model->getPosition();
    auto screenPos = camera.worldToScreen(worldPos);
    rectangle.setPosition(screenPos.x, screenPos.y);
    
    window.draw(rectangle);
}

} // namespace pacman::representation