#include "representation/views/WallView.h"

namespace pacman::representation {

WallView::WallView(pacman::Wall& model, std::weak_ptr<pacman::Camera> camera)  // & niet *
    : EntityView(model, camera), wallModel(model) {
    setupRectangle();
}

void WallView::setupRectangle() {
    auto cam = camera.lock();  // ✅ Lock weak_ptr
    if (!cam) return;

    float size = cam->getSpriteSize();

    float sizeWithOverlap = size + 1.0f;

    rectangle.setSize(sf::Vector2f(sizeWithOverlap, sizeWithOverlap));
    rectangle.setFillColor(sf::Color::Blue);

    rectangle.setOrigin(sizeWithOverlap / 2.0f, sizeWithOverlap / 2.0f);
}

void WallView::draw(sf::RenderWindow& window) {
    auto cam = camera.lock();  // ✅ Lock weak_ptr
    if (!cam) return;

    auto worldPos = model.getPosition();
    auto screenPos = cam->worldToScreen(worldPos);
    rectangle.setPosition(screenPos.x, screenPos.y);
    
    window.draw(rectangle);
}

} // namespace pacman::representation