// src/representation/EntityView.h (NIEUW BESTAND)
#ifndef ENTITYVIEW_H
#define ENTITYVIEW_H

#include <SFML/Graphics.hpp>
#include "SpriteManager.h"
#include "Direction.h"
#include <memory>

namespace PacMan {

    class EntityView {
    public:
        EntityView();
        virtual ~EntityView() = default;

        virtual void update(float deltaTime) = 0;
        virtual void draw(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& scale);

        void setVisible(bool visible) { m_visible = visible; }
        bool isVisible() const { return m_visible; }

    protected:
        sf::Sprite m_sprite;
        std::shared_ptr<SpriteManager> m_spriteManager;

        float m_animationTime;
        int m_currentFrame;
        bool m_visible;
    };

} // namespace PacMan

#endif // ENTITYVIEW_H