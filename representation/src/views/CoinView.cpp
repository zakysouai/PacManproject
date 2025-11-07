#include "representation/views/CoinView.h"
#include "logic/entities/Coin.h"

namespace pacman::representation {

CoinView::CoinView(pacman::Coin* model, const pacman::Camera& camera)
    : EntityView(model, camera), coinModel(model) {
    setupCircle();
}

void CoinView::setupCircle() {
    float radius = camera.getSpriteSize() * 0.15f;
    circle.setRadius(radius);
    circle.setFillColor(sf::Color::Yellow);
    circle.setOrigin(radius, radius);
}

void CoinView::draw(sf::RenderWindow& window) {
    if (!coinModel->isCollected()) {
        auto worldPos = model->getPosition();
        auto screenPos = camera.worldToScreen(worldPos);
        circle.setPosition(screenPos.x, screenPos.y);
        
        window.draw(circle);
    }
}

} // namespace pacman::representation