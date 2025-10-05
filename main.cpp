#include "Game.h"
#include <iostream>
#include <exception>

int main() {
    try {
        // Create and run the game
        PacMan::Game game(800, 600, "Pac-Man - Advanced Programming 2025");
        game.run();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Game closed successfully!" << std::endl;
    return 0;
}

// DLL FILES IN CMAKE-BUILD-DEBUG DIR PLAATSEN, KOPIEER DIE VAN DE SFML DIR