#pragma once
#include "logic/EntityModel.h"
#include "logic/Camera.h"
#include "logic/patterns/Observer.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace pacman::representation {

class EntityView : public pacman::Observer {
public:
    EntityView(pacman::EntityModel* model, const pacman::Camera& camera);
    virtual ~EntityView() = default;
    
    // Observer interface
    void onNotify(const pacman::Event& event) override;
    
    // Rendering
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void update(float deltaTime);
    
protected:
    pacman::EntityModel* model;  // Non-owning pointer
    const pacman::Camera& camera;
    
    sf::Sprite sprite;
    sf::Texture texture;
    
    // Helper to position sprite based on model position
    void updateSpritePosition();
};

} // namespace pacman::representation