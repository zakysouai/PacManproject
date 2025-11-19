#include "representation/Game.h"
#include "representation/states/MenuState.h"
#include "representation/SpriteManager.h"
#include "logic/utils/Stopwatch.h"
#include <iostream>

namespace pacman::representation {

Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PacMan - Advanced Programming") {

    window.setFramerateLimit(FPS_LIMIT);

    // ✨ IMPORTANT: Initialize sprite system BEFORE creating states
    std::cout << "Initializing sprite system..." << std::endl;
    auto& spriteManager = SpriteManager::getInstance();

    try {
        // Load the sprite sheet
        spriteManager.loadSpriteSheet("../resources/sprites/sprite.png");

        // Initialize all sprites and animations
        spriteManager.initialize();

        std::cout << "Sprite system initialized successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to initialize sprite system: " << e.what() << std::endl;
        std::cerr << "Make sure sprite.png is in resources/sprites/ folder!" << std::endl;
        throw;  // Re-throw to prevent game from starting without sprites
    }
    
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