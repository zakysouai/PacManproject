#include "representation/states/VictoryState.h"
#include "representation/states/MenuState.h"
#include "representation/states/LevelState.h"
#include "logic/Score.h"
#include <iostream>

namespace pacman::representation {

VictoryState::VictoryState(bool won, int finalScore, int currentLevel)
    : playerWon(won), score(finalScore), level(currentLevel),
      enteringName(false) {

    // ✅ MAAK TIJDELIJK SCORE OBJECT VOOR HIGHSCORE CHECK
    if (!won) {  // Alleen bij game over
        pacman::Score tempScore;
        isHighScore = tempScore.isHighScore(finalScore);
    } else {
        isHighScore = false;
    }

    loadFont();
    setupTexts();
}

void VictoryState::onEnter() {
    std::cout << (playerWon ? "Level Complete!" : "Game Over!") << std::endl;
    if (isHighScore) {
        std::cout << "NEW HIGH SCORE!" << std::endl;
        enteringName = true;
    }
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
    float centerX = 500.0f;

    // ✅ RESULT TEXT
    resultText.setFont(font);
    if (isHighScore) {
        resultText.setString("NEW HIGH SCORE!");
        resultText.setFillColor(sf::Color::Yellow);
    } else if (playerWon) {
        // ✅ ONDERSCHEID TUTORIAL VS NORMALE LEVEL
        if (level == 0) {
            resultText.setString("TUTORIAL COMPLETE!");
        } else {
            resultText.setString("LEVEL COMPLETE!");
        }
        resultText.setFillColor(sf::Color::Green);
    } else {
        resultText.setString("GAME OVER!");
        resultText.setFillColor(sf::Color::Red);
    }
    resultText.setCharacterSize(72);
    sf::FloatRect resultBounds = resultText.getLocalBounds();
    resultText.setOrigin(resultBounds.width / 2.0f, resultBounds.height / 2.0f);
    resultText.setPosition(centerX, 100);

    // ✅ SCORE TEXT
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::Yellow);
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreBounds.width / 2.0f, scoreBounds.height / 2.0f);
    scoreText.setPosition(centerX, 200);

    // ✅ NAAM INPUT (alleen als highscore)
    if (isHighScore) {
        namePromptText.setFont(font);
        namePromptText.setString("Enter your name:");
        namePromptText.setCharacterSize(32);
        namePromptText.setFillColor(sf::Color::White);
        sf::FloatRect promptBounds = namePromptText.getLocalBounds();
        namePromptText.setOrigin(promptBounds.width / 2.0f, 0);
        namePromptText.setPosition(centerX, 280);

        inputBox.setSize(sf::Vector2f(300, 50));
        inputBox.setFillColor(sf::Color(50, 50, 50));
        inputBox.setOutlineColor(sf::Color::White);
        inputBox.setOutlineThickness(2);
        inputBox.setPosition(centerX - 150, 330);

        nameInputText.setFont(font);
        nameInputText.setString("");
        nameInputText.setCharacterSize(32);
        nameInputText.setFillColor(sf::Color::White);
        nameInputText.setPosition(centerX - 140, 340);

        instructionsText.setFont(font);
        instructionsText.setString("Press ENTER to save");
        instructionsText.setCharacterSize(24);
        instructionsText.setFillColor(sf::Color(180, 180, 180));
    } else {
        instructionsText.setFont(font);

        // ✅ VERSCHILLENDE INSTRUCTIES PER SITUATIE
        if (level == 0 && playerWon) {
            // Tutorial complete - speciale opties
            instructionsText.setString(
                "Press SPACE to start the game\n"
                "Press M for Main Menu"
            );
        } else if (playerWon) {
            // Normale level complete
            instructionsText.setString(
                "Press SPACE for next level\n"
                "Press M for Menu"
            );
        } else {
            // Game over
            instructionsText.setString(
                "Press SPACE to try again\n"
                "Press M for Menu"
            );
        }

        instructionsText.setCharacterSize(24);
        instructionsText.setFillColor(sf::Color::White);
    }

    sf::FloatRect instrBounds = instructionsText.getLocalBounds();
    instructionsText.setOrigin(instrBounds.width / 2.0f, 0);
    instructionsText.setPosition(centerX, 420);
}

void VictoryState::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (enteringName) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b') {
                if (!playerName.empty()) {
                    playerName.pop_back();
                    nameInputText.setString(playerName);
                }
            } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                if (!playerName.empty()) {
                    saveHighScore();
                    enteringName = false;
                    finish(StateAction::SWITCH, std::make_unique<MenuState>());
                }
            } else if (event.text.unicode < 128 && playerName.size() < 12) {
                playerName += static_cast<char>(event.text.unicode);
                nameInputText.setString(playerName);
            }
        }
    } else {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                if (playerWon) {
                    if (level == 0) {
                        finish(StateAction::SWITCH, std::make_unique<LevelState>(1, false));
                    } else {
                        finish(StateAction::SWITCH, std::make_unique<LevelState>(level + 1, false));
                    }
                } else {
                    finish(StateAction::SWITCH, std::make_unique<LevelState>(1, false));
                }
            } else if (event.key.code == sf::Keyboard::M) {
                finish(StateAction::SWITCH, std::make_unique<MenuState>());
            }
        }
    }
}

void VictoryState::saveHighScore() {
    // ✅ MAAK NIEUW SCORE OBJECT OM TE SAVEN
    pacman::Score scoreManager;
    scoreManager.saveHighScore(playerName, score);
    std::cout << "Saved highscore: " << playerName << " - " << score << std::endl;
}

void VictoryState::update(float deltaTime) {
    // Nothing to update
}

void VictoryState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    window.draw(resultText);
    window.draw(scoreText);

    if (enteringName) {
        window.draw(namePromptText);
        window.draw(inputBox);
        window.draw(nameInputText);
    }

    window.draw(instructionsText);
}

} // namespace pacman::representation