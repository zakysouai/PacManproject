// src/utils/SpriteManager.h (NIEUW BESTAND)
#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

namespace PacMan {

enum class SpriteType {
    // PacMan sprites (4 richtingen × 3 animatie frames)
    PacManRight0, PacManRight1, PacManRight2,
    PacManLeft0, PacManLeft1, PacManLeft2,
    PacManUp0, PacManUp1, PacManUp2,
    PacManDown0, PacManDown1, PacManDown2,
    PacManDeath0, PacManDeath1, PacManDeath2, PacManDeath3,

    // Red Ghost (Blinky) - 4 richtingen × 2 animatie frames
    GhostRedRight0, GhostRedRight1,
    GhostRedLeft0, GhostRedLeft1,
    GhostRedUp0, GhostRedUp1,
    GhostRedDown0, GhostRedDown1,

    // Pink Ghost (Pinky)
    GhostPinkRight0, GhostPinkRight1,
    GhostPinkLeft0, GhostPinkLeft1,
    GhostPinkUp0, GhostPinkUp1,
    GhostPinkDown0, GhostPinkDown1,

    // Blue Ghost (Inky)
    GhostBlueRight0, GhostBlueRight1,
    GhostBlueLeft0, GhostBlueLeft1,
    GhostBlueUp0, GhostBlueUp1,
    GhostBlueDown0, GhostBlueDown1,

    // Orange Ghost (Clyde)
    GhostOrangeRight0, GhostOrangeRight1,
    GhostOrangeLeft0, GhostOrangeLeft1,
    GhostOrangeUp0, GhostOrangeUp1,
    GhostOrangeDown0, GhostOrangeDown1,

    // Fear mode ghosts (blue)
    GhostFear0, GhostFear1,
    GhostFearEnd0, GhostFearEnd1, // Knippert wit/blauw

    // Ghost eyes (after being eaten)
    GhostEyesRight, GhostEyesLeft, GhostEyesUp, GhostEyesDown,

    // Fruits
    Cherry, Strawberry, Orange, Apple, Melon, Galaxian, Bell, Key,

    // Score numbers
    Score200, Score400, Score800, Score1600
};

enum class GhostColor {
    Red,
    Pink,
    Blue,
    Orange
};

class SpriteManager {
public:
    static std::shared_ptr<SpriteManager> getInstance();

    bool loadSpriteSheet(const std::string& filename);
    sf::IntRect getSpriteRect(SpriteType type) const;
    const sf::Texture& getTexture() const { return m_spriteSheet; }

    SpriteManager(const SpriteManager&) = delete;
    SpriteManager& operator=(const SpriteManager&) = delete;
    ~SpriteManager() = default;

private:
    SpriteManager();
    void initializeSpriteMap();

    static std::shared_ptr<SpriteManager> s_instance;
    sf::Texture m_spriteSheet;
    std::map<SpriteType, sf::IntRect> m_spriteRects;

    // Sprite dimensies voor 936x939 sheet
    // Typische Pac-Man sprites zijn ~16x16 pixels
    static constexpr int SPRITE_SIZE = 16;
    static constexpr int SPRITE_WIDTH = 16;
    static constexpr int SPRITE_HEIGHT = 16;
};

} // namespace PacMan

#endif // SPRITEMANAGER_H