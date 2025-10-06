// src/representation/PacManView.h (NIEUW BESTAND)
#ifndef PACMANVIEW_H
#define PACMANVIEW_H

#include "EntityView.h"
#include "Direction.h"

namespace PacMan {

    class PacManView : public EntityView {
    public:
        PacManView();

        void update(float deltaTime) override;
        void setDirection(Logic::Direction dir);
        void playDeathAnimation();
        bool isDeathAnimationPlaying() const { return m_playingDeathAnimation; }

    private:
        void updateAnimation(float deltaTime);
        void updateDeathAnimation(float deltaTime);
        SpriteType getCurrentSpriteType() const;

        Logic::Direction m_direction;
        bool m_playingDeathAnimation;

        static constexpr float FRAME_TIME = 0.08f;  // 80ms per frame
        static constexpr int NUM_FRAMES = 3;
        static constexpr float DEATH_FRAME_TIME = 0.15f;
        static constexpr int DEATH_FRAMES = 4;
    };

} // namespace PacMan

#endif // PACMANVIEW_H