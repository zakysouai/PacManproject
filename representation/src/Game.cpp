#include "representation/Game.h"
#include "representation/states/MenuState.h"
#include "logic/utils/Stopwatch.h"
#include <iostream>

namespace pacman::representation {

Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PacMan - Advanced Programming") {

    std::cout << "Game wordt gecalled" << std::endl;

    window.setFramerateLimit(FPS_LIMIT);
    
    // Start with MenuState
    stateManager.pushState(std::make_unique<MenuState>());
}

void Game::run() {
    auto& stopwatch = pacman::Stopwatch::getInstance();
    stopwatch.reset();
    
    while (window.isOpen() && !stateManager.isEmpty()) {
        stopwatch.update();
        float deltaTime = stopwatch.getDeltaTime();
        
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        stateManager.handleInput(event);
    }
}

void Game::update(float deltaTime) {
    stateManager.update(deltaTime);
}

void Game::render() {
    window.clear();
    stateManager.render(window);
    window.display();
}

} // namespace pacman::representation