#ifndef GAME_H
#define GAME_H

#include "StateManager.h"
#include "Stopwatch.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace PacMan {

    /**
     * @brief Main game class managing the game loop and window
     *
     * Responsible for:
     * - Creating and managing the SFML window
     * - Running the main game loop
     * - Managing the StateManager
     */
    class Game {
    public:
        /**
         * @brief Construct the Game with window dimensions
         * @param width Window width in pixels
         * @param height Window height in pixels
         * @param title Window title
         */
        Game(unsigned int width = 800, unsigned int height = 600,
             const std::string& title = "Pac-Man");

        ~Game() = default;

        /**
         * @brief Run the game
         *
         * Starts the main game loop which continues until the window is closed
         */
        void run();

    private:
        /**
         * @brief Process window events
         */
        void processEvents();

        /**
         * @brief Update game logic
         * @param deltaTime Time elapsed since last update
         */
        void update(float deltaTime);

        /**
         * @brief Render the current game state
         */
        void render();

        sf::RenderWindow m_window;
        StateManager m_stateManager;
        std::shared_ptr<Stopwatch> m_stopwatch;

        const unsigned int FPS_LIMIT = 60;
    };

} // namespace PacMan

#endif // GAME_H