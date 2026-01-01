#pragma once
#include "EntityView.h"
#include "logic/entities/Fruit.h"

namespace pacman::representation {

/**
 * @brief View voor Fruit - sprite-based rendering
 *
 * === VISUAL ===
 * Gebruikt sprite "fruit" uit sprite sheet.
 * Groter dan coins, kleiner dan PacMan.
 *
 * === RENDERING ===
 * Alleen tekenen als !fruit.isCollected().
 * Na collection wordt fruit onzichtbaar.
 */
class FruitView : public EntityView {
public:
    FruitView(pacman::Fruit& model, std::weak_ptr<pacman::Camera> camera);
    ~FruitView() override = default;

    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;

private:
    pacman::Fruit& fruitModel;  // Type-safe access

    /**
     * @brief Laad sprite vanuit SpriteManager
     *
     * Setup texture en initial sprite rect.
     */
    void loadSprite();

    /**
     * @brief Update sprite rect en scale
     *
     * Lookup "fruit" rect in SpriteManager,
     * scale op basis van Camera::getSpriteSize().
     */
    void updateSpriteFromTexture();
};

} // namespace pacman::representation