#include "Game.h"
#include "MenuState.h"
#include "Stopwatch.h"

namespace PacMan {

    Game::Game(unsigned int width, unsigned int height, const std::string& title)
        : m_window(sf::VideoMode(width, height), title) {

        // Set frame rate limit
        m_window.setFramerateLimit(FPS_LIMIT);

        // Initialize stopwatch
        m_stopwatch = Stopwatch::getInstance();

        // Start with menu state
        m_stateManager.pushState(std::make_unique<MenuState>());
    }

    void Game::run() {
        while (m_window.isOpen() && m_stateManager.hasStates()) {
            // Update stopwatch for deltaTime
            m_stopwatch->tick();
            float deltaTime = m_stopwatch->getDeltaTime();

            processEvents();
            update(deltaTime);
            render();
        }
    }

    void Game::processEvents() {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }

            // Let current state handle the event
            m_stateManager.handleInput(event);
        }
    }

    void Game::update(float deltaTime) {
        m_stateManager.update(deltaTime);
    }

    void Game::render() {
        m_stateManager.render(m_window);
    }

} // namespace PacMan