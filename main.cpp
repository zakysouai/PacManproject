#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML 2.5.1 Test");

    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(375, 275);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    std::cout << "SFML 2.5.1 werkt!" << std::endl;
    return 0;
}