#include "representation/views/DoorView.h"

namespace pacman::representation {

DoorView::DoorView(std::weak_ptr<pacman::Camera> camera, const pacman::Position& doorPos)  // ✅ CHANGED
    : EntityView(nullptr, camera), doorPosition(doorPos) {

    auto cam = camera.lock();  // ✅ Lock weak_ptr
    if (!cam) return;

    float size = cam->getSpriteSize() + 1.0f;
    rectangle.setSize(sf::Vector2f(size, size));
    rectangle.setFillColor(sf::Color(139, 69, 19));  // Brown door
    rectangle.setOrigin(size / 2.0f, size / 2.0f);
}

void DoorView::draw(sf::RenderWindow& window) {
    auto cam = camera.lock();  // ✅ Lock weak_ptr
    if (!cam) return;

    auto screenPos = cam->worldToScreen(doorPosition);
    rectangle.setPosition(screenPos.x, screenPos.y);
    window.draw(rectangle);
}

} // namespace pacman::representation