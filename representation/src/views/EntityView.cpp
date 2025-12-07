#include "representation/views/EntityView.h"

namespace pacman::representation {

EntityView::EntityView(pacman::EntityModel* model, const pacman::Camera& camera)
    : model(model), camera(camera) {
}

void EntityView::onNotify(const pacman::Event& event) {
    // ✅ BASE IMPLEMENTATION - update sprite position
    if (event.type == pacman::EventType::ENTITY_UPDATED) {
        updateSpritePosition();
    }
}

void EntityView::update(float deltaTime) {
    // ❌ DEPRECATED - wordt niet meer aangeroepen
    // Views worden nu via Observer pattern geüpdate
}

void EntityView::updateSpritePosition() {
    if (!model) return;
    
    auto worldPos = model->getPosition();
    auto screenPos = camera.worldToScreen(worldPos);
    
    sprite.setPosition(screenPos.x, screenPos.y);
}

} // namespace pacman::representation