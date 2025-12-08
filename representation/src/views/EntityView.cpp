#include "representation/views/EntityView.h"

namespace pacman::representation {

EntityView::EntityView(pacman::EntityModel* model, std::weak_ptr<pacman::Camera> camera)  // ✅ CHANGED
    : model(model), camera(camera) {
}

void EntityView::onNotify(const pacman::Event& event) {
    if (event.type == pacman::EventType::ENTITY_UPDATED) {
        updateSpritePosition();
    }
}

void EntityView::update(float deltaTime) {
    // Deprecated
}

void EntityView::updateSpritePosition() {
    if (!model) return;

    auto cam = camera.lock();  // ✅ Lock weak_ptr
    if (!cam) return;

    auto worldPos = model->getPosition();
    auto screenPos = cam->worldToScreen(worldPos);
    
    sprite.setPosition(screenPos.x, screenPos.y);
}

} // namespace pacman::representation