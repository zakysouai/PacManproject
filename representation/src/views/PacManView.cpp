#include "representation/views/PacManView.h"
#include <iostream>

namespace pacman::representation {

PacManView::PacManView(pacman::PacMan* model, const pacman::Camera& camera)
    : EntityView(model, camera), pacmanModel(model) {
    loadTexture();
}

void PacManView::loadTexture() {
    // TODO: Load sprite sheet
    // For now, create a simple yellow circle
    // texture.loadFromFile("resources/sprites/pacman.png");
    // sprite.setTexture(texture);
}

void PacManView::update(float deltaTime) {
    EntityView::update(deltaTime);  // Update position
    updateAnimation(deltaTime);
    updateSpriteDirection();
}

void PacManView::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    if (animationTimer >= animationSpeed) {
        animationTimer = 0.0f;
        currentFrame = (currentFrame + 1) % totalFrames;
    }
}

void PacManView::updateSpriteDirection() {
    // TODO: Rotate sprite based on pacmanModel->getDirection()
    // For now, just update position
}

void PacManView::draw(sf::RenderWindow& window) {
    if (!pacmanModel) return;

    // For now, draw a simple yellow circle
    auto worldPos = pacmanModel->getPosition();
    auto screenPos = camera.worldToScreen(worldPos);

    float radius = camera.getSpriteSize() * 0.4f;
    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::Yellow);
    circle.setOrigin(radius, radius);
    circle.setPosition(screenPos.x, screenPos.y);

    window.draw(circle);
}

void PacManView::onNotify(const pacman::Event& event) {
    // Handle events like direction changes, death, etc.
    switch (event.type) {
    case pacman::EventType::DIRECTION_CHANGED:
        updateSpriteDirection();
        break;
    case pacman::EventType::PACMAN_DIED:
        std::cout << "PacMan died!" << std::endl;
        break;
    default:
        break;
    }
}

} // namespace pacman::representation