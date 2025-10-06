// src/representation/GhostView.h (NIEUW BESTAND)
#ifndef GHOSTVIEW_H
#define GHOSTVIEW_H

#include "EntityView.h"
#include "Direction.h"

namespace PacMan {

    class GhostView : public EntityView {
    public:
        GhostView(GhostColor color);

        void update(float deltaTime) override;
        void setDirection(Logic::Direction dir);
        void setFearMode(bool fear);
        void setFearModeEnding(bool ending);
        void setEatenMode(bool eaten);

    private:
        void updateAnimation(float deltaTime);
        SpriteType getCurrentSpriteType() const;
        SpriteType getGhostSprite(int frame) const;

        GhostColor m_color;
        Logic::Direction m_direction;
        bool m_fearMode;
        bool m_fearModeEnding;
        bool m_eatenMode;

        static constexpr float FRAME_TIME = 0.15f;
        static constexpr int NUM_FRAMES = 2;
    };

} // namespace PacMan

#endif // GHOSTVIEW_H