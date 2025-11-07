#include "representation/views/EntityView.h"

namespace pacman::representation {

EntityView::EntityView(pacman::EntityModel* model, const pacman::Camera& camera)
    : model(model), camera(camera) {
}

void EntityView::onNotify(const pacman::Event& event) {
    // Base implementation - can be overridden
}

void EntityView::update(float deltaTime) {
    updateSpritePosition();
}

void EntityView::updateSpritePosition() {
    if (!model) return;
    
    auto worldPos = model->getPosition();
    auto screenPos = camera.worldToScreen(worldPos);
    
    sprite.setPosition(screenPos.x, screenPos.y);
}

} // namespace pacman::representation