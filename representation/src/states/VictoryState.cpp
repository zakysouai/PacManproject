// representation/src/states/VictoryState.cpp
#include "representation/states/VictoryState.h"
#include "representation/states/MenuState.h"
#include "representation/states/LevelState.h"
#include <iostream>

namespace pacman::representation {

VictoryState::VictoryState(bool won, int finalScore, int currentLevel)
    : playerWon(won), score(finalScore), level(currentLevel) {
    loadFont();
    setupTexts();
}

void VictoryState::onEnter() {
    std::cout << (playerWon ? "Level Complete!" : "Game Over!") << std::endl;
}

void VictoryState::onExit() {
    std::cout << "Leaving Victory/GameOver screen" << std::endl;
}

void VictoryState::loadFont() {
    if (!font.loadFromFile("../resources/fonts/Retro.ttf")) {
        std::cerr << "Warning: Could not load font" << std::endl;
    }
}

void VictoryState::setupTexts() {
    float windowWidth = 1000.0f;
    float windowHeight = 600.0f;
    float centerX = windowWidth / 2.0f;

    // ✅ RESULT TEXT - GECENTREERD
    resultText.setFont(font);
    if (playerWon) {
        resultText.setString("LEVEL COMPLETE!");
        resultText.setFillColor(sf::Color::Green);
    } else {
        resultText.setString("GAME OVER!");
        resultText.setFillColor(sf::Color::Red);
    }
    resultText.setCharacterSize(72);

    sf::FloatRect resultBounds = resultText.getLocalBounds();
    resultText.setOrigin(resultBounds.width / 2.0f, resultBounds.height / 2.0f);
    resultText.setPosition(centerX, 150);

    // ✅ SCORE TEXT - GECENTREERD
    scoreText.setFont(font);
    scoreText.setString("Final Score: " + std::to_string(score));
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::Yellow);

    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreBounds.width / 2.0f, scoreBounds.height / 2.0f);
    scoreText.setPosition(centerX, 280);

    // ✅ INSTRUCTIONS TEXT - GECENTREERD
    instructionsText.setFont(font);
    if (playerWon) {
        instructionsText.setString("Press SPACE for next level\nPress M for Menu");
    } else {
        instructionsText.setString("Press SPACE to try again\nPress M for Menu");
    }
    instructionsText.setCharacterSize(24);
    instructionsText.setFillColor(sf::Color::White);

    sf::FloatRect instrBounds = instructionsText.getLocalBounds();
    instructionsText.setOrigin(instrBounds.width / 2.0f, 0);
    instructionsText.setPosition(centerX, 400);
}

void VictoryState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space) {
            if (playerWon) {
                finish(StateAction::SWITCH, std::make_unique<LevelState>(level + 1));
            } else {
                finish(StateAction::SWITCH, std::make_unique<LevelState>(1));
            }
        } else if (event.key.code == sf::Keyboard::M) {
            finish(StateAction::SWITCH, std::make_unique<MenuState>());
        }
    }
}

void VictoryState::update([[maybe_unused]] float deltaTime) {
    // Nothing to update
}

void VictoryState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    window.draw(resultText);
    window.draw(scoreText);
    window.draw(instructionsText);
}

} // namespace pacman::representation