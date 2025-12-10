#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

namespace pacman::representation {

class PausedState : public State {
public:
    PausedState();
    ~PausedState() override = default;
    
    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
private:
    sf::Font font;
    sf::Text pausedText;
    sf::Text instructionsText;
    sf::RectangleShape overlay;  // Semi-transparent background
    
    void loadFont();
    void setupTexts();
};

} // namespace pacman::representation