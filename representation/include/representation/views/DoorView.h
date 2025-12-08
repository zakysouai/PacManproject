#pragma once
#include "EntityView.h"

namespace pacman::representation {

class DoorView : public EntityView {
public:
    DoorView(std::weak_ptr<pacman::Camera> camera, const pacman::Position& doorPos);  // GEEN model parameter
    ~DoorView() override = default;

    void draw(sf::RenderWindow& window) override;
    void onNotify(const pacman::Event&) override {}  // Lege implementatie, gebruikt geen events
    
private:
    pacman::Position doorPosition;
    sf::RectangleShape rectangle;
};

} // namespace pacman::representation