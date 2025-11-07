#include "representation/states/VictoryState.h"
#include "representation/states/MenuState.h"
#include "representation/states/LevelState.h"
#include <iostream>

namespace pacman::representation {

VictoryState::VictoryState(bool won, int finalScore)
    : playerWon(won), score(finalScore) {
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
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "Warning: Could not load font" << std::endl;
    }
}

void VictoryState::setupTexts() {
    // Result text (Win or Lose)
    resultText.setFont(font);
    if (playerWon) {
        resultText.setString("LEVEL COMPLETE!");
        resultText.setFillColor(sf::Color::Green);
    } else {
        resultText.setString("GAME OVER!");
        resultText.setFillColor(sf::Color::Red);
    }
    resultText.setCharacterSize(64);
    resultText.setPosition(200, 150);
    
    // Score text
    scoreText.setFont(font);
    scoreText.setString("Final Score: " + std::to_string(score));
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(250, 280);
    
    // Instructions text
    instructionsText.setFont(font);
    if (playerWon) {
        instructionsText.setString("Press SPACE for next level\nPress M for Menu");
    } else {
        instructionsText.setString("Press SPACE to try again\nPress M for Menu");
    }
    instructionsText.setCharacterSize(24);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setPosition(220, 400);
}

void VictoryState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space) {
            if (playerWon) {
                // Go to next level
                finish(StateAction::SWITCH, std::make_unique<LevelState>(2));  // TODO: Pass actual next level
            } else {
                // Restart from level 1
                finish(StateAction::SWITCH, std::make_unique<LevelState>(1));
            }
        } else if (event.key.code == sf::Keyboard::M) {
            // Go to menu
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