#include "representation/views/GhostView.h"

namespace pacman::representation {

GhostView::GhostView(pacman::Ghost* model, const pacman::Camera& camera, pacman::GhostColor color)
    : EntityView(model, camera), ghostModel(model), ghostColor(color) {

    float radius = camera.getSpriteSize() * 0.4f;
    circle.setRadius(radius);
    circle.setOrigin(radius, radius);

    // Color based on type
    switch (color) {
    case pacman::GhostColor::RED:
        circle.setFillColor(sf::Color::Red);
        break;
    case pacman::GhostColor::PINK:
        circle.setFillColor(sf::Color::Magenta);
        break;
    case pacman::GhostColor::BLUE:
        circle.setFillColor(sf::Color::Cyan);
        break;
    case pacman::GhostColor::ORANGE:
        circle.setFillColor(sf::Color(255, 165, 0));
        break;
    }
}

void GhostView::draw(sf::RenderWindow& window) {
    auto worldPos = model->getPosition();
    auto screenPos = camera.worldToScreen(worldPos);
    circle.setPosition(screenPos.x, screenPos.y);

    window.draw(circle);
}

} // namespace pacman::representation