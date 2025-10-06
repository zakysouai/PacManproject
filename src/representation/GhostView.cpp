// src/representation/GhostView.cpp (NIEUW BESTAND)
#include "GhostView.h"

namespace PacMan {

GhostView::GhostView(GhostColor color)
    : EntityView()
    , m_color(color)
    , m_direction(Logic::Direction::Right)
    , m_fearMode(false)
    , m_fearModeEnding(false)
    , m_eatenMode(false) {

    // Zet initiële sprite
    m_sprite.setTextureRect(m_spriteManager->getSpriteRect(getGhostSprite(0)));
}

void GhostView::update(float deltaTime) {
    updateAnimation(deltaTime);
}

void GhostView::updateAnimation(float deltaTime) {
    m_animationTime += deltaTime;

    if (m_animationTime >= FRAME_TIME) {
        m_animationTime = 0.0f;
        m_currentFrame = (m_currentFrame + 1) % NUM_FRAMES;

        // Update texture rect
        m_sprite.setTextureRect(m_spriteManager->getSpriteRect(getCurrentSpriteType()));
    }
}

SpriteType GhostView::getCurrentSpriteType() const {
    // Als de ghost opgegeten is, toon alleen ogen
    if (m_eatenMode) {
        switch (m_direction) {
            case Logic::Direction::Right: return SpriteType::GhostEyesRight;
            case Logic::Direction::Left: return SpriteType::GhostEyesLeft;
            case Logic::Direction::Up: return SpriteType::GhostEyesUp;
            case Logic::Direction::Down: return SpriteType::GhostEyesDown;
            default: return SpriteType::GhostEyesRight;
        }
    }

    // Fear mode sprites
    if (m_fearMode) {
        if (m_fearModeEnding) {
            return m_currentFrame == 0 ? SpriteType::GhostFearEnd0 : SpriteType::GhostFearEnd1;
        }
        return m_currentFrame == 0 ? SpriteType::GhostFear0 : SpriteType::GhostFear1;
    }

    // Normale ghost sprites
    return getGhostSprite(m_currentFrame);
}

SpriteType GhostView::getGhostSprite(int frame) const {
    // Bepaal de basis sprite type gebaseerd op kleur en richting
    switch (m_color) {
        case GhostColor::Red:
            switch (m_direction) {
                case Logic::Direction::Right:
                    return frame == 0 ? SpriteType::GhostRedRight0 : SpriteType::GhostRedRight1;
                case Logic::Direction::Left:
                    return frame == 0 ? SpriteType::GhostRedLeft0 : SpriteType::GhostRedLeft1;
                case Logic::Direction::Up:
                    return frame == 0 ? SpriteType::GhostRedUp0 : SpriteType::GhostRedUp1;
                case Logic::Direction::Down:
                    return frame == 0 ? SpriteType::GhostRedDown0 : SpriteType::GhostRedDown1;
                default:
                    return SpriteType::GhostRedRight0;
            }

        case GhostColor::Pink:
            switch (m_direction) {
                case Logic::Direction::Right:
                    return frame == 0 ? SpriteType::GhostPinkRight0 : SpriteType::GhostPinkRight1;
                case Logic::Direction::Left:
                    return frame == 0 ? SpriteType::GhostPinkLeft0 : SpriteType::GhostPinkLeft1;
                case Logic::Direction::Up:
                    return frame == 0 ? SpriteType::GhostPinkUp0 : SpriteType::GhostPinkUp1;
                case Logic::Direction::Down:
                    return frame == 0 ? SpriteType::GhostPinkDown0 : SpriteType::GhostPinkDown1;
                default:
                    return SpriteType::GhostPinkRight0;
            }

        case GhostColor::Blue:
            switch (m_direction) {
                case Logic::Direction::Right:
                    return frame == 0 ? SpriteType::GhostBlueRight0 : SpriteType::GhostBlueRight1;
                case Logic::Direction::Left:
                    return frame == 0 ? SpriteType::GhostBlueLeft0 : SpriteType::GhostBlueLeft1;
                case Logic::Direction::Up:
                    return frame == 0 ? SpriteType::GhostBlueUp0 : SpriteType::GhostBlueUp1;
                case Logic::Direction::Down:
                    return frame == 0 ? SpriteType::GhostBlueDown0 : SpriteType::GhostBlueDown1;
                default:
                    return SpriteType::GhostBlueRight0;
            }

        case GhostColor::Orange:
            switch (m_direction) {
                case Logic::Direction::Right:
                    return frame == 0 ? SpriteType::GhostOrangeRight0 : SpriteType::GhostOrangeRight1;
                case Logic::Direction::Left:
                    return frame == 0 ? SpriteType::GhostOrangeLeft0 : SpriteType::GhostOrangeLeft1;
                case Logic::Direction::Up:
                    return frame == 0 ? SpriteType::GhostOrangeUp0 : SpriteType::GhostOrangeUp1;
                case Logic::Direction::Down:
                    return frame == 0 ? SpriteType::GhostOrangeDown0 : SpriteType::GhostOrangeDown1;
                default:
                    return SpriteType::GhostOrangeRight0;
            }
    }

    return SpriteType::GhostRedRight0;
}

void GhostView::setDirection(Logic::Direction dir) {
    if (m_direction != dir && dir != Logic::Direction::None) {
        m_direction = dir;
    }
}

void GhostView::setFearMode(bool fear) {
    m_fearMode = fear;
    if (fear) {
        m_eatenMode = false;
    }
}

void GhostView::setFearModeEnding(bool ending) {
    m_fearModeEnding = ending;
}

void GhostView::setEatenMode(bool eaten) {
    m_eatenMode = eaten;
    if (eaten) {
        m_fearMode = false;
    }
}

} // namespace PacMan