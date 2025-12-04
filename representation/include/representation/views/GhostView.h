#pragma once
#include "EntityView.h"
#include "logic/entities/Ghost.h"

namespace pacman::representation {

class GhostView : public EntityView {
public:
    GhostView(pacman::Ghost* model, const pacman::Camera& camera, pacman::GhostColor color);
    ~GhostView() override = default;

    void draw(sf::RenderWindow& window) override;

private:
    pacman::Ghost* ghostModel;
    pacman::GhostColor ghostColor;
    sf::CircleShape circle;  // Simpel voor nu
};

} // namespace pacman::representation