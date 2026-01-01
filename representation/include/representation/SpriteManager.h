#pragma once
#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace pacman::representation {

/**
 * @brief Singleton manager voor sprites en animaties
 *
 * === VERANTWOORDELIJKHEDEN ===
 * 1. Sprite sheet loading (1x bij startup)
 * 2. Sprite rectangle definitions (positie van sprites in sheet)
 * 3. Animation definitions (welke sprites maken animatie)
 * 4. Centralized access voor alle Views
 *
 * === SINGLETON PATTERN ===
 * Waarom singleton:
 * - Sprite sheet moet 1x geladen (memory efficiency)
 * - Alle Views gebruiken dezelfde texture
 * - Global definitions voor sprites/animations
 *
 * === USAGE ===
 * ```cpp
 * // In Game::Game():
 * auto& mgr = SpriteManager::getInstance();
 * mgr.loadSpriteSheet("../resources/sprites/sprite.png");
 * mgr.initialize();  // Roept defineAll{Sprites,Animations} aan
 *
 * // In View constructor:
 * sprite.setTexture(mgr.getTexture());
 * sprite.setTextureRect(mgr.getSpriteRect("pacman_right_open"));
 *
 * // In View animation:
 * animController.play(mgr.getAnimation("pacman_walk_right"));
 * ```
 *
 * === SPRITE SHEET FORMAT ===
 * Eén PNG met alle sprites in grid layout.
 * Sprite rectangles gedefinieerd via sf::IntRect(left, top, width, height).
 * Coördinaten handmatig extracted via spritecow.com.
 */
class SpriteManager {
public:
    /**
     * @brief Verkrijg singleton instance (Meyers Singleton)
     * @return Reference naar enige SpriteManager instance
     */
    static SpriteManager& getInstance() {
        static SpriteManager instance;
        return instance;
    }

    // Delete copy/move (singleton)
    SpriteManager(const SpriteManager&) = delete;
    SpriteManager& operator=(const SpriteManager&) = delete;
    SpriteManager(SpriteManager&&) = delete;
    SpriteManager& operator=(SpriteManager&&) = delete;

    /**
     * @brief Laad sprite sheet texture
     * @param path Pad naar PNG file
     * @throws std::runtime_error als file niet geladen kan worden
     *
     * Roep 1x aan bij game startup.
     * Texture blijft in memory voor duur van game.
     */
    void loadSpriteSheet(const std::string& path);

    /**
     * @brief Verkrijg sprite sheet texture
     * @return Const reference naar texture
     *
     * Alle Views sharen deze texture (memory efficient).
     */
    const sf::Texture& getTexture() const { return spriteSheet; }

    bool isTextureLoaded() const { return textureLoaded; }

    /**
     * @brief Definieer sprite rectangle
     * @param name Unieke naam (e.g., "pacman_right_open")
     * @param rect Positie in sprite sheet (pixels)
     *
     * IntRect format: (left, top, width, height)
     * Coördinaten komen van spritecow.com.
     */
    void defineSpriteRect(const std::string& name, const sf::IntRect& rect);

    /**
     * @brief Verkrijg sprite rectangle
     * @param name Sprite naam
     * @return Rectangle voor deze sprite
     * @throws std::out_of_range als sprite niet bestaat
     */
    sf::IntRect getSpriteRect(const std::string& name) const;

    bool hasSpriteRect(const std::string& name) const;

    /**
     * @brief Definieer animation
     * @param name Unieke naam (e.g., "pacman_walk_right")
     * @param animation Animation object (frame sequence + timing)
     */
    void defineAnimation(const std::string& name, const Animation& animation);

    /**
     * @brief Verkrijg animation
     * @param name Animation naam
     * @return Const reference naar animation
     * @throws std::out_of_range als animation niet bestaat
     */
    const Animation& getAnimation(const std::string& name) const;

    bool hasAnimation(const std::string& name) const;

    /**
     * @brief Initialiseer alle sprites en animaties
     *
     * Roept aan:
     * - defineAllSprites() (alle sprite rectangles)
     * - defineAllAnimations() (alle animations)
     *
     * Wordt automatisch aangeroepen in constructor,
     * maar kan ook manueel voor re-initialization.
     */
    void initialize();

private:
    /**
     * @brief Private constructor (singleton)
     */
    SpriteManager();
    ~SpriteManager() = default;

    /**
     * @brief Definieer alle sprite rectangles
     *
     * Hard-coded sprite positions extracted via spritecow.com.
     * Sprites voor:
     * - PacMan (4 directions × 3 frames + death animation)
     * - Ghosts (4 colors × 4 directions × 2 frames + scared)
     * - Collectibles (coin, fruit)
     */
    void defineAllSprites();

    /**
     * @brief Definieer alle animations
     *
     * Animations voor:
     * - PacMan walking (4 directions)
     * - PacMan death (11 frames, non-looping)
     * - Ghost walking (4 colors × 4 directions)
     * - Ghost scared (1 animation voor alle ghosts)
     */
    void defineAllAnimations();

    // Data
    sf::Texture spriteSheet;                           // Loaded sprite sheet
    bool textureLoaded = false;
    std::map<std::string, sf::IntRect> spriteRects;    // name → rectangle mapping
    std::map<std::string, Animation> animations;       // name → animation mapping
};

} // namespace pacman::representation