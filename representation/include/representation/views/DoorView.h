#pragma once
#include "EntityView.h"

namespace pacman::representation {

class DoorView : public EntityView {
public:
    DoorView(const pacman::Camera& camera, const pacman::Position& doorPos);
    ~DoorView() override = default;
    
    void draw(sf::RenderWindow& window) override;
    void onNotify(const pacman::Event& event) override {}
    
private:
    pacman::Position doorPosition;
    sf::RectangleShape rectangle;
};

} // namespace pacman::representation