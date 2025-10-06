// src/representation/EntityView.cpp (NIEUW BESTAND)
#include "EntityView.h"

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

        m_sprite.setPosition(position);
        m_sprite.setScale(scale);

        // Centreer de sprite op de positie
        sf::IntRect rect = m_sprite.getTextureRect();
        m_sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);

        window.draw(m_sprite);
    }

} // namespace PacMan