#include "representation/include/representation/Game.h"
#include <iostream>
#include <exception>

int main() {
    try {
        pacman::representation::Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

// TODO: UI regelen en mooier maken (hardcoded coordinaten)
// TODO: de normale map.txt gebruiken als soort 'tutorial' en dan naar map_big switchen als 2de level?
// TODO: checken of we wel correct werken met observers etc...
// TODO: checken voor memleaks (valgrind)
// TODO: checken voor correcte use of smart pointers
// TODO: highscores implementeren
// TODO: bug in 1 van de hoeken waar pacman vast zit
// TODO: deftige death animation
// TODO: ghosts gaan te snel uit spawn na 1 death van pacman