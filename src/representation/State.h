#ifndef STATE_H
#define STATE_H

#include <memory>
#include <SFML/Graphics.hpp>

namespace PacMan {

    class StateManager;

    /**
     * @brief Abstract base class for all game states
     *
     * Implements the State pattern to manage different game screens
     * (menu, gameplay, pause, victory, etc.)
     */
    class State {
    public:
        virtual ~State() = default;

        /**
         * @brief Called when state becomes active
         */
        virtual void onEnter() = 0;

        /**
         * @brief Called when state becomes inactive
         */
        virtual void onExit() = 0;

        /**
         * @brief Handle user input events
         * @param event SFML event to process
         * @param manager StateManager for potential state transitions
         */
        virtual void handleInput(const sf::Event& event, StateManager& manager) = 0;

        /**
         * @brief Update game logic
         * @param deltaTime Time elapsed since last update
         * @param manager StateManager for potential state transitions
         */
        virtual void update(float deltaTime, StateManager& manager) = 0;

        /**
         * @brief Render the state
         * @param window Window to render to
         */
        virtual void render(sf::RenderWindow& window) = 0;

    protected:
        State() = default;
    };

} // namespace PacMan

#endif // STATE_H