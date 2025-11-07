#include "representation/include/representation/Game.h"
#include <iostream>
#include <exception>

int main() {
    try {
        pacman::representation::Game game;
        game.run();
        std::cout << "nieuwe main" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}