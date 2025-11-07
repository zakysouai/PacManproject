#pragma once
#include "EntityView.h"
#include "logic/entities/PacMan.h"

namespace pacman::representation {

class PacManView : public EntityView {
public:
    PacManView(pacman::PacMan* model, const pacman::Camera& camera);
    ~PacManView() override = default;
    
    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void onNotify(const pacman::Event& event) override;
    
private:
    pacman::PacMan* pacmanModel;
    
    // Animation
    float animationTimer = 0.0f;
    float animationSpeed = 0.1f;
    int currentFrame = 0;
    int totalFrames = 2;  // Open and closed mouth
    
    void loadTexture();
    void updateAnimation(float deltaTime);
    void updateSpriteDirection();
};

} // namespace pacman::representation