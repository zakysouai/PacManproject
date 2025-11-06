// src/utils/SpriteManager.cpp - VERSIE 2 met 16×16 ghost sprites
// Probeer deze als de 14×14 versie niet werkt!
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

    m_spriteSheet.setSmooth(false);

    std::cout << "Sprite sheet loaded successfully: " << filename << std::endl;
    std::cout << "Texture size: " << m_spriteSheet.getSize().x << "x"
              << m_spriteSheet.getSize().y << std::endl;

    return true;
}

void SpriteManager::initializeSpriteMap() {
    std::cout << "Initializing sprite map with LARGER ghost rectangles..." << std::endl;

    // ========== PAC-MAN SPRITES (Deze zijn correct!) ==========
    m_spriteRects[SpriteType::PacManRight0] = sf::IntRect(858, 0, 13, 13);
    m_spriteRects[SpriteType::PacManRight1] = sf::IntRect(874, 0, 13, 13);
    m_spriteRects[SpriteType::PacManRight2] = sf::IntRect(890, 0, 13, 13);

    m_spriteRects[SpriteType::PacManLeft0] = sf::IntRect(858, 16, 13, 13);
    m_spriteRects[SpriteType::PacManLeft1] = sf::IntRect(874, 16, 13, 13);
    m_spriteRects[SpriteType::PacManLeft2] = sf::IntRect(890, 16, 13, 13);

    m_spriteRects[SpriteType::PacManUp0] = sf::IntRect(858, 32, 13, 13);
    m_spriteRects[SpriteType::PacManUp1] = sf::IntRect(874, 32, 13, 13);
    m_spriteRects[SpriteType::PacManUp2] = sf::IntRect(890, 32, 13, 13);

    m_spriteRects[SpriteType::PacManDown0] = sf::IntRect(858, 48, 13, 13);
    m_spriteRects[SpriteType::PacManDown1] = sf::IntRect(874, 48, 13, 13);
    m_spriteRects[SpriteType::PacManDown2] = sf::IntRect(890, 48, 13, 13);

    m_spriteRects[SpriteType::PacManDeath0] = sf::IntRect(906, 0, 13, 13);
    m_spriteRects[SpriteType::PacManDeath1] = sf::IntRect(906, 16, 13, 13);
    m_spriteRects[SpriteType::PacManDeath2] = sf::IntRect(906, 32, 13, 13);
    m_spriteRects[SpriteType::PacManDeath3] = sf::IntRect(906, 48, 13, 13);

    // ========== GHOST SPRITES - PROBEER 16×16 ==========
    // HYPOTHESE: Ghosts zijn eigenlijk 16×16, niet 14×14!

    const int GHOST_W = 16;  // <-- VERGROOT van 14 naar 16
    const int GHOST_H = 16;  // <-- VERGROOT van 14 naar 16
    const int GHOST_SPACING = 16;

    std::cout << "Ghost sprite size: " << GHOST_W << "x" << GHOST_H << std::endl;

    // RED GHOST
    m_spriteRects[SpriteType::GhostRedRight0] = sf::IntRect(0, 0, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostRedRight1] = sf::IntRect(16, 0, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostRedLeft0] = sf::IntRect(32, 0, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostRedLeft1] = sf::IntRect(48, 0, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostRedUp0] = sf::IntRect(64, 0, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostRedUp1] = sf::IntRect(80, 0, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostRedDown0] = sf::IntRect(96, 0, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostRedDown1] = sf::IntRect(112, 0, GHOST_W, GHOST_H);

    // PINK GHOST
    m_spriteRects[SpriteType::GhostPinkRight0] = sf::IntRect(0, 16, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostPinkRight1] = sf::IntRect(16, 16, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostPinkLeft0] = sf::IntRect(32, 16, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostPinkLeft1] = sf::IntRect(48, 16, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostPinkUp0] = sf::IntRect(64, 16, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostPinkUp1] = sf::IntRect(80, 16, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostPinkDown0] = sf::IntRect(96, 16, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostPinkDown1] = sf::IntRect(112, 16, GHOST_W, GHOST_H);

    // BLUE GHOST
    m_spriteRects[SpriteType::GhostBlueRight0] = sf::IntRect(0, 32, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostBlueRight1] = sf::IntRect(16, 32, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostBlueLeft0] = sf::IntRect(32, 32, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostBlueLeft1] = sf::IntRect(48, 32, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostBlueUp0] = sf::IntRect(64, 32, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostBlueUp1] = sf::IntRect(80, 32, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostBlueDown0] = sf::IntRect(96, 32, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostBlueDown1] = sf::IntRect(112, 32, GHOST_W, GHOST_H);

    // ORANGE GHOST
    m_spriteRects[SpriteType::GhostOrangeRight0] = sf::IntRect(0, 48, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostOrangeRight1] = sf::IntRect(16, 48, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostOrangeLeft0] = sf::IntRect(32, 48, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostOrangeLeft1] = sf::IntRect(48, 48, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostOrangeUp0] = sf::IntRect(64, 48, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostOrangeUp1] = sf::IntRect(80, 48, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostOrangeDown0] = sf::IntRect(96, 48, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostOrangeDown1] = sf::IntRect(112, 48, GHOST_W, GHOST_H);

    // FEAR MODE (ook 16×16)
    m_spriteRects[SpriteType::GhostFear0] = sf::IntRect(304, 64, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostFear1] = sf::IntRect(320, 64, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostFearEnd0] = sf::IntRect(304, 80, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostFearEnd1] = sf::IntRect(320, 80, GHOST_W, GHOST_H);

    // EYES (ook 16×16)
    m_spriteRects[SpriteType::GhostEyesRight] = sf::IntRect(336, 64, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostEyesLeft] = sf::IntRect(352, 64, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostEyesUp] = sf::IntRect(336, 80, GHOST_W, GHOST_H);
    m_spriteRects[SpriteType::GhostEyesDown] = sf::IntRect(352, 80, GHOST_W, GHOST_H);

    // FRUITS
    m_spriteRects[SpriteType::Cherry] = sf::IntRect(488, 48, 13, 13);
    m_spriteRects[SpriteType::Strawberry] = sf::IntRect(504, 48, 13, 13);
    m_spriteRects[SpriteType::Orange] = sf::IntRect(520, 64, 13, 13);
    m_spriteRects[SpriteType::Apple] = sf::IntRect(536, 80, 13, 13);
    m_spriteRects[SpriteType::Melon] = sf::IntRect(552, 96, 13, 13);
    m_spriteRects[SpriteType::Galaxian] = sf::IntRect(568, 112, 13, 13);
    m_spriteRects[SpriteType::Bell] = sf::IntRect(584, 128, 13, 13);
    m_spriteRects[SpriteType::Key] = sf::IntRect(600, 144, 13, 13);

    // SCORE NUMBERS
    m_spriteRects[SpriteType::Score200] = sf::IntRect(456, 128, 16, 8);
    m_spriteRects[SpriteType::Score400] = sf::IntRect(472, 128, 16, 8);
    m_spriteRects[SpriteType::Score800] = sf::IntRect(488, 128, 16, 8);
    m_spriteRects[SpriteType::Score1600] = sf::IntRect(504, 128, 16, 8);

    std::cout << "Sprite rectangles initialized!" << std::endl;
}

sf::IntRect SpriteManager::getSpriteRect(SpriteType type) const {
    auto it = m_spriteRects.find(type);
    if (it != m_spriteRects.end()) {
        return it->second;
    }

    std::cerr << "Warning: Sprite type not found" << std::endl;
    return sf::IntRect(858, 0, 13, 13);
}

} // namespace PacMan