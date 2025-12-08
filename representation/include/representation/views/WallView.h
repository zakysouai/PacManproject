#pragma once
#include "EntityView.h"
#include "logic/entities/Wall.h"

namespace pacman::representation {

class WallView : public EntityView {
public:
    WallView(pacman::Wall& model, std::weak_ptr<pacman::Camera> camera);
    ~WallView() override = default;
    
    void draw(sf::RenderWindow& window) override;
    
private:
    pacman::Wall* wallModel;
    sf::RectangleShape rectangle;
    
    void setupRectangle();
};

} // namespace pacman::representation