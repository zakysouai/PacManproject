#pragma once
#include "State.h"
#include "logic/Score.h"
#include <SFML/Graphics.hpp>

namespace pacman::representation {

/**
 * @brief VictoryState - win/lose screen met highscore input
 *
 * === MODES ===
 * Game Over (playerWon = false):
 * - Title: "GAME OVER!"
 * - Highscore check: mogelijk naam invoer
 * - Options: Space = retry, M = menu
 *
 * Level Complete (playerWon = true):
 * - Title: "LEVEL COMPLETE!" of "TUTORIAL COMPLETE!"
 * - Options: Space = next level, M = menu
 *
 * New Highscore (isHighScore = true):
 * - Title: "NEW HIGH SCORE!"
 * - Naam input box (max 12 chars)
 * - Enter = save & menu
 *
 * === HIGHSCORE FLOW ===
 * 1. Constructor: check Score::isHighScore(finalScore)
 * 2. onEnter(): if highscore, set enteringName = true
 * 3. TextEntered events: build playerName string
 * 4. Enter pressed: saveHighScore(), finish(SWITCH, MenuState)
 *
 * === TEXT INPUT ===
 * SFML TextEntered event:
 * - Backspace (unicode '\b'): remove char
 * - Enter ('\r' of '\n'): submit
 * - ASCII chars: append to playerName (max 12)
 */
class VictoryState : public State {
public:
    /**
     * @brief Constructor
     * @param won true = level complete, false = game over
     * @param finalScore Score behaald
     * @param currentLevel Level nummer (0 = tutorial)
     */
    VictoryState(bool won, int finalScore, int currentLevel);
    ~VictoryState() override = default;

    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    bool playerWon;
    int score;
    int level;

    // Highscore input state
    bool isHighScore;
    bool enteringName;
    std::string playerName;

    sf::Font font;
    sf::Text resultText;       // "LEVEL COMPLETE!" / "GAME OVER!" / "NEW HIGH SCORE!"
    sf::Text scoreText;        // "Score: 12345"
    sf::Text instructionsText;

    // Naam input UI
    sf::Text namePromptText;   // "Enter your name:"
    sf::Text nameInputText;    // Live typing feedback
    sf::RectangleShape inputBox;

    void loadFont();

    /**
     * @brief Setup text op basis van win/lose/highscore state
     *
     * Conditional UI:
     * - If highscore: show naam input
     * - Else: show instructions (space/M)
     */
    void setupTexts();

    /**
     * @brief Save highscore naar file
     *
     * Gebruikt Score::saveHighScore(playerName, score).
     */
    void saveHighScore();
};

} // namespace pacman::representation