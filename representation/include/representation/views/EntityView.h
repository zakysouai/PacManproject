#pragma once
#include "logic/EntityModel.h"
#include "logic/Camera.h"
#include "logic/patterns/Observer.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace pacman::representation {

class EntityView : public pacman::Observer {
public:
    EntityView(pacman::EntityModel& model, std::weak_ptr<pacman::Camera> camera);  // ✅ CHANGED
    virtual ~EntityView() = default;

    void onNotify(const pacman::Event& event) override;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual void update(float deltaTime);

protected:
    pacman::EntityModel& model;
    std::weak_ptr<pacman::Camera> camera;  // ✅ CHANGED from const Camera&

    sf::Sprite sprite;

    void updateSpritePosition();
};

} // namespace pacman::representation