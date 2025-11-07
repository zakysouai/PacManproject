#include "representation/views/WallView.h"

namespace pacman::representation {

WallView::WallView(pacman::Wall* model, const pacman::Camera& camera)
    : EntityView(model, camera), wallModel(model) {
    setupRectangle();
}

void WallView::setupRectangle() {
    float size = camera.getSpriteSize();

    // Make walls slightly larger to ensure no gaps (add 1 pixel overlap)
    float sizeWithOverlap = size + 1.0f;

    rectangle.setSize(sf::Vector2f(sizeWithOverlap, sizeWithOverlap));
    rectangle.setFillColor(sf::Color::Blue);

    // Center the origin
    rectangle.setOrigin(sizeWithOverlap / 2.0f, sizeWithOverlap / 2.0f);
}

void WallView::draw(sf::RenderWindow& window) {
    auto worldPos = model->getPosition();
    auto screenPos = camera.worldToScreen(worldPos);
    rectangle.setPosition(screenPos.x, screenPos.y);
    
    window.draw(rectangle);
}

} // namespace pacman::representation