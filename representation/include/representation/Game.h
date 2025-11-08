#pragma once
#include "representation/states/StateManager.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace pacman::representation {

class Game {
public:
    Game();
    ~Game() = default;
    
    void run();
    
private:
    sf::RenderWindow window;
    StateManager stateManager;
    
    void processEvents();
    void update(float deltaTime);
    void render();
    
    // Configuration
    static constexpr int WINDOW_WIDTH = 1000;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int FPS_LIMIT = 60;
};

} // namespace pacman::representation