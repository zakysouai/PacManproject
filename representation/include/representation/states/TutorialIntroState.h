#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

namespace pacman::representation {

/**
 * @brief TutorialIntroState - tutorial instructies scherm
 *
 * === UI ===
 * - Title: "TUTORIAL"
 * - Instructions: Objectives, ghost behavior, controls
 * - Continue prompt: "Press ENTER to start" (blink effect)
 *
 * === FORMATTING ===
 * Structured text met sections:
 * - OBJECTIVE: Collect coins/fruits
 * - AVOID GHOSTS: Lose life on contact
 * - POWER-UPS: Fruits enable eating ghosts
 * - CONTROLS: Arrow keys, ESC
 *
 * === TRANSITION ===
 * Enter → SWITCH LevelState(0, true)  // Tutorial level
 * ESC → POP (terug naar MenuState)
 *
 * === BLINK EFFECT ===
 * "Press ENTER" text alpha oscillates:
 * alpha = 127 + 127 * sin(elapsed * 3.0)
 */
class TutorialIntroState : public State {
public:
    TutorialIntroState();
    ~TutorialIntroState() override = default;

    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text instructionsText;  // Multi-line formatted text
    sf::Text controlsText;      // Separate controls box
    sf::Text continueText;      // Blinking "Press ENTER"

    void loadFont();

    /**
     * @brief Setup text layout
     *
     * - Title: top center
     * - Instructions: left-aligned (200, 150)
     * - Controls: right sidebar (600, 390)
     * - Continue: bottom center (blink)
     */
    void setupTexts();
};

} // namespace pacman::representation