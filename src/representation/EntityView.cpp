// src/representation/EntityView.cpp - DEBUG VERSIE
#include "EntityView.h"
#include <iostream>

namespace PacMan {

    EntityView::EntityView()
        : m_animationTime(0.0f)
        , m_currentFrame(0)
        , m_visible(true) {

        m_spriteManager = SpriteManager::getInstance();
        m_sprite.setTexture(m_spriteManager->getTexture());
    }

    void EntityView::draw(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& scale) {
        if (!m_visible) return;

        // DEBUG: Print sprite info eerste keer
        static bool firstTime = true;
        if (firstTime) {
            sf::IntRect rect = m_sprite.getTextureRect();
            std::cout << "=== SPRITE DEBUG INFO ===" << std::endl;
            std::cout << "TextureRect: (" << rect.left << ", " << rect.top
                      << ", " << rect.width << ", " << rect.height << ")" << std::endl;
            std::cout << "Position: (" << position.x << ", " << position.y << ")" << std::endl;
            std::cout << "Scale: (" << scale.x << ", " << scale.y << ")" << std::endl;
            std::cout << "Final size: " << (rect.width * scale.x) << "x"
                      << (rect.height * scale.y) << " pixels" << std::endl;
            std::cout << "=========================" << std::endl;
            firstTime = false;
        }

        // BELANGRIJK: Zet origin VOOR scale en position!
        sf::IntRect rect = m_sprite.getTextureRect();
        m_sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);

        // Dan pas scale en positie
        m_sprite.setScale(scale);
        m_sprite.setPosition(position);

        window.draw(m_sprite);
    }

} // namespace PacMan