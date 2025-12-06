#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

namespace pacman::representation {

class VictoryState : public State {
public:
    VictoryState(bool won, int finalScore);
    ~VictoryState() override = default;
    
    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    VictoryState(bool won, int finalScore, int currentLevel = 1);
    
private:
    bool playerWon;
    int score;
    int level;
    
    sf::Font font;
    sf::Text resultText;
    sf::Text scoreText;
    sf::Text instructionsText;
    
    void loadFont();
    void setupTexts();
};

} // namespace pacman::representation