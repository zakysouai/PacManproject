#include "representation/views/GhostView.h"
#include <iostream>

namespace pacman::representation {

GhostView::GhostView(pacman::Ghost* model, const pacman::Camera& camera)
    : EntityView(model, camera), ghostModel(model) {
    loadTexture();
}

void GhostView::loadTexture() {
    // TODO: Load sprite sheet
    // texture.loadFromFile("resources/sprites/ghosts.png");
    // sprite.setTexture(texture);
}

void GhostView::update(float deltaTime) {
    EntityView::update(deltaTime);  // Update position
    updateAnimation(deltaTime);
    updateColor();
}

void GhostView::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    
    if (animationTimer >= 0.2f) {  // Animation speed
        animationTimer = 0.0f;
        currentFrame = (currentFrame + 1) % 2;  // 2 frames
    }
}

void GhostView::updateColor() {
    // Change color based on mode (fear mode = blue)
    // This will be applied when drawing
}

void GhostView::draw(sf::RenderWindow& window) {
    if (!ghostModel) return;
    
    auto worldPos = ghostModel->getPosition();
    auto screenPos = camera.worldToScreen(worldPos);
    
    float radius = camera.getSpriteSize() * 0.4f;
    sf::CircleShape circle(radius);
    
    // Color based on ghost type and mode
    if (ghostModel->isFeared()) {
        circle.setFillColor(sf::Color::Blue);
    } else {
        // Different colors for different ghost types
        switch (ghostModel->getType()) {
            case pacman::GhostType::RANDOM:
                circle.setFillColor(sf::Color::Red);
                break;
            case pacman::GhostType::CHASER:
                circle.setFillColor(sf::Color::Magenta);
                break;
            case pacman::GhostType::PREDICTOR:
                circle.setFillColor(sf::Color::Cyan);
                break;
            default:
                circle.setFillColor(sf::Color::Red);
                break;
        }
    }
    
    circle.setOrigin(radius, radius);
    circle.setPosition(screenPos.x, screenPos.y);
    
    window.draw(circle);
}

void GhostView::onNotify(const pacman::Event& event) {
    // Handle ghost-specific events
    EntityView::onNotify(event);
}

} // namespace pacman::representation