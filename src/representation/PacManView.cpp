// src/representation/PacManView.cpp (NIEUW BESTAND)
#include "PacManView.h"

namespace PacMan {

PacManView::PacManView()
    : EntityView()
    , m_direction(Logic::Direction::Right)
    , m_playingDeathAnimation(false) {

    // Zet initiële sprite
    m_sprite.setTextureRect(m_spriteManager->getSpriteRect(SpriteType::PacManRight1));
}

void PacManView::update(float deltaTime) {
    if (m_playingDeathAnimation) {
        updateDeathAnimation(deltaTime);
    } else {
        updateAnimation(deltaTime);
    }
}

void PacManView::updateAnimation(float deltaTime) {
    m_animationTime += deltaTime;

    if (m_animationTime >= FRAME_TIME) {
        m_animationTime = 0.0f;
        m_currentFrame = (m_currentFrame + 1) % NUM_FRAMES;

        // Update texture rect
        m_sprite.setTextureRect(m_spriteManager->getSpriteRect(getCurrentSpriteType()));
    }
}

void PacManView::updateDeathAnimation(float deltaTime) {
    m_animationTime += deltaTime;

    if (m_animationTime >= DEATH_FRAME_TIME) {
        m_animationTime = 0.0f;
        m_currentFrame++;

        if (m_currentFrame >= DEATH_FRAMES) {
            m_playingDeathAnimation = false;
            m_currentFrame = 0;
            return;
        }

        // Update death sprite
        SpriteType deathSprite;
        switch (m_currentFrame) {
            case 0: deathSprite = SpriteType::PacManDeath0; break;
            case 1: deathSprite = SpriteType::PacManDeath1; break;
            case 2: deathSprite = SpriteType::PacManDeath2; break;
            case 3: deathSprite = SpriteType::PacManDeath3; break;
            default: deathSprite = SpriteType::PacManDeath0; break;
        }
        m_sprite.setTextureRect(m_spriteManager->getSpriteRect(deathSprite));
    }
}

SpriteType PacManView::getCurrentSpriteType() const {
    switch (m_direction) {
        case Logic::Direction::Right:
            if (m_currentFrame == 0) return SpriteType::PacManRight0;
            if (m_currentFrame == 1) return SpriteType::PacManRight1;
            return SpriteType::PacManRight2;

        case Logic::Direction::Left:
            if (m_currentFrame == 0) return SpriteType::PacManLeft0;
            if (m_currentFrame == 1) return SpriteType::PacManLeft1;
            return SpriteType::PacManLeft2;

        case Logic::Direction::Up:
            if (m_currentFrame == 0) return SpriteType::PacManUp0;
            if (m_currentFrame == 1) return SpriteType::PacManUp1;
            return SpriteType::PacManUp2;

        case Logic::Direction::Down:
            if (m_currentFrame == 0) return SpriteType::PacManDown0;
            if (m_currentFrame == 1) return SpriteType::PacManDown1;
            return SpriteType::PacManDown2;

        default:
            return SpriteType::PacManRight1;
    }
}

void PacManView::setDirection(Logic::Direction dir) {
    if (m_direction != dir && dir != Logic::Direction::None) {
        m_direction = dir;
        m_currentFrame = 0;  // Reset animatie bij richtingsverandering
        m_animationTime = 0.0f;
    }
}

void PacManView::playDeathAnimation() {
    m_playingDeathAnimation = true;
    m_currentFrame = 0;
    m_animationTime = 0.0f;
}

} // namespace PacMan