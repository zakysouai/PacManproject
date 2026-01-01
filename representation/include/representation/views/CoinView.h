#pragma once
#include "EntityView.h"
#include "logic/entities/Coin.h"

namespace pacman::representation {

/**
 * @brief View voor Coin - simple yellow circle
 *
 * === VISUAL ===
 * sf::CircleShape (geen sprite sheet):
 * - Kleur: Yellow
 * - Radius: 0.15 * spriteSize (klein)
 * - Origin: gecentreerd
 *
 * === RENDERING ===
 * Alleen tekenen als !coin.isCollected().
 * Na collection wordt coin onzichtbaar (blijft wel in World).
 */
class CoinView : public EntityView {
public:
    CoinView(pacman::Coin& model, std::weak_ptr<pacman::Camera> camera);
    ~CoinView() override = default;

    void draw(sf::RenderWindow& window) override;

private:
    pacman::Coin& coinModel;  // Type-safe access
    sf::CircleShape circle;   // SFML shape (geen sprite)

    /**
     * @brief Initialiseer circle shape
     *
     * Zet radius, color, origin op basis van Camera::getSpriteSize().
     */
    void setupCircle();
};

} // namespace pacman::representation