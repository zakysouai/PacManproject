#pragma once
#include "State.h"
#include "logic/Score.h"
#include <SFML/Graphics.hpp>

namespace pacman::representation {

class VictoryState : public State {
public:
    VictoryState(bool won, int finalScore, int currentLevel);
    ~VictoryState() override = default;

    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    bool playerWon;
    int score;
    int level;
    pacman::Score* scoreManager;

    // ✅ NAAM INPUT
    bool isHighScore;
    bool enteringName;
    std::string playerName;

    sf::Font font;
    sf::Text resultText;
    sf::Text scoreText;
    sf::Text instructionsText;

    // ✅ NAAM INPUT UI
    sf::Text namePromptText;
    sf::Text nameInputText;
    sf::RectangleShape inputBox;

    void loadFont();
    void setupTexts();
    void saveHighScore();
};

} // namespace pacman::representation