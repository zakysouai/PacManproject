#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "State.h"
#include <vector>
#include <memory>

namespace PacMan {

    /**
     * @brief Manages game states using a stack-based approach
     *
     * Implements a stack of states to allow pausing/resuming.
     * Only the top state is active at any given time.
     */
    class StateManager {
    public:
        StateManager() = default;
        ~StateManager() = default;

        /**
         * @brief Push a new state onto the stack
         * @param state Unique pointer to the new state
         */
        void pushState(std::unique_ptr<State> state);

        /**
         * @brief Pop the current state from the stack
         */
        void popState();

        /**
         * @brief Replace current state with a new one
         * @param state Unique pointer to the new state
         */
        void changeState(std::unique_ptr<State> state);

        /**
         * @brief Handle input for the current state
         * @param event SFML event to process
         */
        void handleInput(const sf::Event& event);

        /**
         * @brief Update the current state
         * @param deltaTime Time elapsed since last update
         */
        void update(float deltaTime);

        /**
         * @brief Render the current state
         * @param window Window to render to
         */
        void render(sf::RenderWindow& window);

        /**
         * @brief Check if there are any states
         * @return true if state stack is not empty
         */
        bool hasStates() const { return !m_states.empty(); }

    private:
        std::vector<std::unique_ptr<State>> m_states;
    };

} // namespace PacMan

#endif // STATEMANAGER_H