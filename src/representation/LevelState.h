// src/representation/LevelState.h (UPDATE)
#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include "State.h"
#include "World.h"
#include "Camera.h"
#include "PacManView.h"
#include "GhostView.h"
#include <memory>
#include <vector>

namespace PacMan {

    class LevelState : public State {
    public:
        LevelState();
        ~LevelState() override = default;

        void onEnter() override;
        void onExit() override;
        void handleInput(const sf::Event& event, StateManager& manager) override;
        void update(float deltaTime, StateManager& manager) override;
        void render(sf::RenderWindow& window) override;

    private:
        void renderMaze(sf::RenderWindow& window);
        void renderEntities(sf::RenderWindow& window);

        std::unique_ptr<Logic::World> m_world;
        std::unique_ptr<Camera> m_camera;

        // Entity views
        std::unique_ptr<PacManView> m_pacmanView;
        std::vector<std::unique_ptr<GhostView>> m_ghostViews;
    };

} // namespace PacMan

#endif // LEVELSTATE_H