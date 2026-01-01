#pragma once
#include "representation/states/StateManager.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace pacman::representation {

/**
 * @brief Hoofd game klasse - SFML window en main loop
 *
 * Verantwoordelijkheden:
 * - SFML window initialisatie
 * - Sprite system setup (SpriteManager singleton)
 * - Main game loop (event processing, update, render)
 * - StateManager lifecycle
 *
 * === GAME LOOP ===
 * while (window open && states niet leeg):
 *   1. Stopwatch update (deltaTime)
 *   2. Event processing (input, window close)
 *   3. State update (game logic)
 *   4. Render (draw to window)
 *   5. Display (swap buffers)
 *
 * === STARTUP SEQUENCE ===
 * 1. Window creation (1000x600)
 * 2. Framerate cap (60 FPS max)
 * 3. Sprite sheet loading
 * 4. Sprite/animation definitions
 * 5. Push MenuState
 */
class Game {
public:
    /**
     * @brief Constructor - initialiseer SFML en sprite system
     * @throws std::runtime_error als sprite sheet niet geladen kan worden
     */
    Game();
    ~Game() = default;

    /**
     * @brief Start main game loop
     *
     * Blijft draaien tot:
     * - Window gesloten wordt
     * - StateManager leeg is (geen states meer)
     */
    void run();

private:
    sf::RenderWindow window;
    StateManager stateManager;

    /**
     * @brief Verwerk SFML events (input, window close)
     *
     * Delegates input naar huidige State via StateManager.
     */
    void processEvents();

    /**
     * @brief Update game logic
     * @param deltaTime Tijd sinds laatste frame
     */
    void update(float deltaTime);

    /**
     * @brief Render huidige state
     */
    void render();

    // Window configuratie
    static constexpr int WINDOW_WIDTH = 1000;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int FPS_LIMIT = 60;
};

} // namespace pacman::representation