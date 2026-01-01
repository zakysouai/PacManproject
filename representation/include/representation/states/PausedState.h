#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

namespace pacman::representation {

/**
 * @brief PausedState - pause overlay over LevelState
 *
 * === UI ===
 * - Semi-transparent zwart overlay (200 alpha)
 * - "PAUSED" text (groot, geel, gecentreerd)
 * - Instructions: "ESC to Resume" / "M for Menu"
 *
 * === RENDERING ===
 * GEEN window.clear() → LevelState blijft zichtbaar onder overlay.
 *
 * === STATE STACK ===
 * Stack tijdens pause:
 * ```
 * [PausedState]   ← active (rendering overlay)
 * [LevelState]    ← suspended (maar visible)
 * [MenuState]
 * ```
 *
 * === TRANSITIONS ===
 * - ESC: POP (terug naar LevelState)
 * - M: SWITCH MenuState (verwijder PausedState + LevelState)
 */
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

    /**
     * @brief Setup centered text en fullscreen overlay
     *
     * Fixed 1000x600 layout.
     * Overlay: volledige window size.
     */
    void setupTexts();
};

} // namespace pacman::representation