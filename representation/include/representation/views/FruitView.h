#pragma once
#include "EntityView.h"
#include "logic/entities/Fruit.h"

namespace pacman::representation {

class FruitView : public EntityView {
public:
    FruitView(pacman::Fruit* model, const pacman::Camera& camera);
    ~FruitView() override = default;
    
    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;

private:
    pacman::Fruit* fruitModel;

    void loadSprite();
    void updateSpriteFromTexture();
};

} // namespace pacman::representation