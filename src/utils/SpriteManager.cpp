// src/utils/SpriteManager.cpp (NIEUW BESTAND)
#include "SpriteManager.h"
#include <iostream>
#include <stdexcept>

namespace PacMan {

std::shared_ptr<SpriteManager> SpriteManager::s_instance = nullptr;

SpriteManager::SpriteManager() {
    initializeSpriteMap();
}

std::shared_ptr<SpriteManager> SpriteManager::getInstance() {
    if (!s_instance) {
        s_instance = std::shared_ptr<SpriteManager>(new SpriteManager());
    }
    return s_instance;
}

bool SpriteManager::loadSpriteSheet(const std::string& filename) {
    if (!m_spriteSheet.loadFromFile(filename)) {
        std::cerr << "Failed to load sprite sheet: " << filename << std::endl;
        return false;
    }

    // Zet smooth filtering uit voor pixel-perfect rendering
    m_spriteSheet.setSmooth(false);

    std::cout << "Sprite sheet loaded successfully: " << filename << std::endl;
    std::cout << "Texture size: " << m_spriteSheet.getSize().x << "x"
              << m_spriteSheet.getSize().y << std::endl;

    return true;
}

void SpriteManager::initializeSpriteMap() {
    // BELANGRIJKE OPMERKING:
    // Deze coördinaten zijn gebaseerd op een typische Pac-Man sprite sheet layout.
    // Je moet deze aanpassen aan jouw specifieke sprite.png bestand!
    // Open sprite.png in een image editor en meet de exacte posities.

    // ========== PAC-MAN SPRITES ==========
    // Rij 0: PacMan animations
    // Rechts (kolom 0-2)
    m_spriteRects[SpriteType::PacManRight0] = sf::IntRect(456, 0, 13, 13);
    m_spriteRects[SpriteType::PacManRight1] = sf::IntRect(472, 0, 13, 13);
    m_spriteRects[SpriteType::PacManRight2] = sf::IntRect(488, 0, 13, 13);

    // Links (kolom 3-5)
    m_spriteRects[SpriteType::PacManLeft0] = sf::IntRect(456, 16, 13, 13);
    m_spriteRects[SpriteType::PacManLeft1] = sf::IntRect(472, 16, 13, 13);
    m_spriteRects[SpriteType::PacManLeft2] = sf::IntRect(488, 16, 13, 13);

    // Omhoog (kolom 6-8)
    m_spriteRects[SpriteType::PacManUp0] = sf::IntRect(456, 32, 13, 13);
    m_spriteRects[SpriteType::PacManUp1] = sf::IntRect(472, 32, 13, 13);
    m_spriteRects[SpriteType::PacManUp2] = sf::IntRect(488, 32, 13, 13);

    // Omlaag (kolom 9-11)
    m_spriteRects[SpriteType::PacManDown0] = sf::IntRect(456, 48, 13, 13);
    m_spriteRects[SpriteType::PacManDown1] = sf::IntRect(472, 48, 13, 13);
    m_spriteRects[SpriteType::PacManDown2] = sf::IntRect(488, 48, 13, 13);

    // Death animatie
    m_spriteRects[SpriteType::PacManDeath0] = sf::IntRect(504, 0, 13, 13);
    m_spriteRects[SpriteType::PacManDeath1] = sf::IntRect(520, 0, 13, 13);
    m_spriteRects[SpriteType::PacManDeath2] = sf::IntRect(536, 0, 13, 13);
    m_spriteRects[SpriteType::PacManDeath3] = sf::IntRect(552, 0, 13, 13);

    // ========== GHOST SPRITES - RED (BLINKY) ==========
    // Rechts
    m_spriteRects[SpriteType::GhostRedRight0] = sf::IntRect(456, 64, 14, 14);
    m_spriteRects[SpriteType::GhostRedRight1] = sf::IntRect(472, 64, 14, 14);
    // Links
    m_spriteRects[SpriteType::GhostRedLeft0] = sf::IntRect(488, 64, 14, 14);
    m_spriteRects[SpriteType::GhostRedLeft1] = sf::IntRect(504, 64, 14, 14);
    // Omhoog
    m_spriteRects[SpriteType::GhostRedUp0] = sf::IntRect(520, 64, 14, 14);
    m_spriteRects[SpriteType::GhostRedUp1] = sf::IntRect(536, 64, 14, 14);
    // Omlaag
    m_spriteRects[SpriteType::GhostRedDown0] = sf::IntRect(552, 64, 14, 14);
    m_spriteRects[SpriteType::GhostRedDown1] = sf::IntRect(568, 64, 14, 14);

    // ========== GHOST SPRITES - PINK (PINKY) ==========
    m_spriteRects[SpriteType::GhostPinkRight0] = sf::IntRect(456, 80, 14, 14);
    m_spriteRects[SpriteType::GhostPinkRight1] = sf::IntRect(472, 80, 14, 14);
    m_spriteRects[SpriteType::GhostPinkLeft0] = sf::IntRect(488, 80, 14, 14);
    m_spriteRects[SpriteType::GhostPinkLeft1] = sf::IntRect(504, 80, 14, 14);
    m_spriteRects[SpriteType::GhostPinkUp0] = sf::IntRect(520, 80, 14, 14);
    m_spriteRects[SpriteType::GhostPinkUp1] = sf::IntRect(536, 80, 14, 14);
    m_spriteRects[SpriteType::GhostPinkDown0] = sf::IntRect(552, 80, 14, 14);
    m_spriteRects[SpriteType::GhostPinkDown1] = sf::IntRect(568, 80, 14, 14);

    // ========== GHOST SPRITES - BLUE (INKY) ==========
    m_spriteRects[SpriteType::GhostBlueRight0] = sf::IntRect(456, 96, 14, 14);
    m_spriteRects[SpriteType::GhostBlueRight1] = sf::IntRect(472, 96, 14, 14);
    m_spriteRects[SpriteType::GhostBlueLeft0] = sf::IntRect(488, 96, 14, 14);
    m_spriteRects[SpriteType::GhostBlueLeft1] = sf::IntRect(504, 96, 14, 14);
    m_spriteRects[SpriteType::GhostBlueUp0] = sf::IntRect(520, 96, 14, 14);
    m_spriteRects[SpriteType::GhostBlueUp1] = sf::IntRect(536, 96, 14, 14);
    m_spriteRects[SpriteType::GhostBlueDown0] = sf::IntRect(552, 96, 14, 14);
    m_spriteRects[SpriteType::GhostBlueDown1] = sf::IntRect(568, 96, 14, 14);

    // ========== GHOST SPRITES - ORANGE (CLYDE) ==========
    m_spriteRects[SpriteType::GhostOrangeRight0] = sf::IntRect(456, 112, 14, 14);
    m_spriteRects[SpriteType::GhostOrangeRight1] = sf::IntRect(472, 112, 14, 14);
    m_spriteRects[SpriteType::GhostOrangeLeft0] = sf::IntRect(488, 112, 14, 14);
    m_spriteRects[SpriteType::GhostOrangeLeft1] = sf::IntRect(504, 112, 14, 14);
    m_spriteRects[SpriteType::GhostOrangeUp0] = sf::IntRect(520, 112, 14, 14);
    m_spriteRects[SpriteType::GhostOrangeUp1] = sf::IntRect(536, 112, 14, 14);
    m_spriteRects[SpriteType::GhostOrangeDown0] = sf::IntRect(552, 112, 14, 14);
    m_spriteRects[SpriteType::GhostOrangeDown1] = sf::IntRect(568, 112, 14, 14);

    // ========== FEAR MODE GHOSTS ==========
    m_spriteRects[SpriteType::GhostFear0] = sf::IntRect(584, 64, 14, 14);
    m_spriteRects[SpriteType::GhostFear1] = sf::IntRect(600, 64, 14, 14);
    m_spriteRects[SpriteType::GhostFearEnd0] = sf::IntRect(584, 80, 14, 14);
    m_spriteRects[SpriteType::GhostFearEnd1] = sf::IntRect(600, 80, 14, 14);

    // ========== GHOST EYES ==========
    m_spriteRects[SpriteType::GhostEyesRight] = sf::IntRect(584, 96, 14, 14);
    m_spriteRects[SpriteType::GhostEyesLeft] = sf::IntRect(600, 96, 14, 14);
    m_spriteRects[SpriteType::GhostEyesUp] = sf::IntRect(616, 96, 14, 14);
    m_spriteRects[SpriteType::GhostEyesDown] = sf::IntRect(632, 96, 14, 14);

    // ========== FRUITS ==========
    m_spriteRects[SpriteType::Cherry] = sf::IntRect(488, 48, 13, 13);
    m_spriteRects[SpriteType::Strawberry] = sf::IntRect(504, 48, 13, 13);
    m_spriteRects[SpriteType::Orange] = sf::IntRect(520, 48, 13, 13);
    m_spriteRects[SpriteType::Apple] = sf::IntRect(536, 48, 13, 13);
    m_spriteRects[SpriteType::Melon] = sf::IntRect(552, 48, 13, 13);
    m_spriteRects[SpriteType::Galaxian] = sf::IntRect(568, 48, 13, 13);
    m_spriteRects[SpriteType::Bell] = sf::IntRect(584, 48, 13, 13);
    m_spriteRects[SpriteType::Key] = sf::IntRect(600, 48, 13, 13);

    // ========== SCORE NUMBERS ==========
    m_spriteRects[SpriteType::Score200] = sf::IntRect(456, 128, 16, 8);
    m_spriteRects[SpriteType::Score400] = sf::IntRect(472, 128, 16, 8);
    m_spriteRects[SpriteType::Score800] = sf::IntRect(488, 128, 16, 8);
    m_spriteRects[SpriteType::Score1600] = sf::IntRect(504, 128, 16, 8);
}

sf::IntRect SpriteManager::getSpriteRect(SpriteType type) const {
    auto it = m_spriteRects.find(type);
    if (it != m_spriteRects.end()) {
        return it->second;
    }
    // Fallback: return eerste PacMan sprite
    std::cerr << "Warning: Sprite type not found, using default" << std::endl;
    return sf::IntRect(456, 0, 13, 13);
}

} // namespace PacMan