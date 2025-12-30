#include "representation/views/CoinView.h"
#include "logic/entities/Coin.h"

namespace pacman::representation {

CoinView::CoinView(pacman::Coin& model, std::weak_ptr<pacman::Camera> camera)  // & niet *
    : EntityView(model, camera), coinModel(model) {
    setupCircle();
}

void CoinView::setupCircle() {
    auto cam = camera.lock();  // ✅ Lock weak_ptr
    if (!cam) return;

    float radius = cam->getSpriteSize() * 0.15f;
    circle.setRadius(radius);
    circle.setFillColor(sf::Color::Yellow);
    circle.setOrigin(radius, radius);
}

void CoinView::draw(sf::RenderWindow& window) {
    if (!coinModel.isCollected()) {
        auto cam = camera.lock();  // ✅ Lock weak_ptr
        if (!cam) return;

        auto worldPos = model.getPosition();
        auto screenPos = cam->worldToScreen(worldPos);
        circle.setPosition(screenPos.x, screenPos.y);
        
        window.draw(circle);
    }
}

} // namespace pacman::representation