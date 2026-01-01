#pragma once
#include "EntityView.h"
#include "logic/entities/Wall.h"

namespace pacman::representation {

/**
 * @brief View voor Wall - blue rectangle
 *
 * === VISUAL ===
 * sf::RectangleShape (geen sprite):
 * - Kleur: Blue
 * - Size: spriteSize + 1.0px (overlap voor seamless grid)
 * - Origin: gecentreerd
 *
 * === OVERLAP TRICK ===
 * +1.0px voorkomt gaps tussen walls door floating point rounding.
 * Walls overlappen lichtjes voor solid appearance.
 */
class WallView : public EntityView {
public:
    WallView(pacman::Wall& model, std::weak_ptr<pacman::Camera> camera);
    ~WallView() override = default;

    void draw(sf::RenderWindow& window) override;

private:
    pacman::Wall& wallModel;  // Type-safe access
    sf::RectangleShape rectangle;

    /**
     * @brief Initialiseer rectangle shape
     *
     * Zet size (met overlap), color, origin.
     */
    void setupRectangle();
};

} // namespace pacman::representation