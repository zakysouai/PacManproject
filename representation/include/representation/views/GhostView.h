#pragma once
#include "EntityView.h"
#include "logic/entities/Ghost.h"

namespace pacman::representation {

class GhostView : public EntityView {
public:
    GhostView(pacman::Ghost* model, const pacman::Camera& camera);
    ~GhostView() override = default;
    
    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void onNotify(const pacman::Event& event) override;
    
private:
    pacman::Ghost* ghostModel;
    
    // Animation
    float animationTimer = 0.0f;
    int currentFrame = 0;
    
    void loadTexture();
    void updateAnimation(float deltaTime);
    void updateColor();  // For fear mode (blue color)
};

} // namespace pacman::representation