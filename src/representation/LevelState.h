#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include "State.h"
#include "World.h"
#include "Camera.h"
#include <memory>

namespace PacMan {

    /**
     * @brief State representing active gameplay
     */
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

        std::unique_ptr<Logic::World> m_world;
        std::unique_ptr<Camera> m_camera;
    };

} // namespace PacMan

#endif // LEVELSTATE_H