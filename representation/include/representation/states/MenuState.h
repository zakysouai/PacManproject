#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace pacman::representation {

/**
 * @brief MenuState - hoofdmenu met highscores en knoppen
 *
 * === UI ELEMENTEN ===
 * - Title: "PAC-MAN" (groot, geel, gecentreerd)
 * - Play button: Start level 1
 * - Tutorial button: Start tutorial level
 * - Highscore tabel: Top 5 scores
 *
 * === BUTTON INTERACTIE ===
 * Mouse hover:
 * - Button kleur verandert (donkerblauw → lichtblauw)
 * - Scale 1.05x (subtle grow effect)
 *
 * Mouse click:
 * - Play → PUSH LevelState(1, false)
 * - Tutorial → PUSH TutorialIntroState
 *
 * === COORDINATE SYSTEM ===
 * Fixed 1000x600 view (geen camera scaling).
 * UI posities in absolute pixels.
 * Mouse coords: window.mapPixelToCoords() voor view mapping.
 */
class MenuState : public State {
public:
    MenuState();
    ~MenuState() override = default;

    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text titleText;

    // Play button
    sf::RectangleShape playButton;
    sf::Text playButtonText;
    bool playButtonHovered = false;

    // Tutorial button
    sf::RectangleShape tutorialButton;
    sf::Text tutorialButtonText;
    bool tutorialButtonHovered = false;

    // Highscores
    sf::Text highScoresTitle;
    std::vector<sf::Text> highScoreTexts;  // Top 5 entries

    /**
     * @brief Laad Retro.ttf font
     * @throws std::runtime_error als font niet gevonden
     */
    void loadFont();

    /**
     * @brief Setup alle text/button posities
     *
     * Centered layout:
     * - centerX = 500.0f
     * - Vertical spacing tussen elementen
     */
    void setupTexts();

    /**
     * @brief Laad highscores vanuit file
     *
     * Gebruikt Score::getHighScores() voor top 5.
     * Formatted als: "1.  NAME        SCORE"
     */
    void loadHighScores();

    bool isMouseOverButton(const sf::Vector2f& mousePos) const;

    /**
     * @brief Converteer pixel coords naar view coords
     * @param window Window reference
     * @param pixel Pixel position
     * @return Position in view coordinates
     *
     * Nodig voor button hit detection met custom view.
     */
    sf::Vector2f mapPixelToCoords(const sf::RenderWindow& window, sf::Vector2i pixel);
};

} // namespace pacman::representation