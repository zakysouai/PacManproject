#include "representation/views/DoorView.h"

namespace pacman::representation {

DoorView::DoorView(const pacman::Camera& camera, const pacman::Position& doorPos)
    : EntityView(nullptr, camera), doorPosition(doorPos) {
    
    float size = camera.getSpriteSize() + 1.0f;
    rectangle.setSize(sf::Vector2f(size, size));
    rectangle.setFillColor(sf::Color(139, 69, 19));  // Brown door
    rectangle.setOrigin(size / 2.0f, size / 2.0f);
}

void DoorView::draw(sf::RenderWindow& window) {
    auto screenPos = camera.worldToScreen(doorPosition);
    rectangle.setPosition(screenPos.x, screenPos.y);
    window.draw(rectangle);
}

} // namespace pacman::representation