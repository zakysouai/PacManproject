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

// TODO: UI regelen en mooier maken (hardcoded coordinaten fixen voor fullscreen)
// TODO: de normale map.txt gebruiken als soort 'tutorial' en dan naar map_big switchen als 2de level?
// TODO: checken of we wel correct werken met observers etc...
// TODO: checken voor memleaks (valgrind)
// TODO: checken voor correcte use of smart pointers
// TODO: bug in 1 van de hoeken waar pacman vast zit
// TODO: deftige death animation
// TODO: eerst een bevroren beeld van de map. met READY in het midden voor 2 seconden ofzo
