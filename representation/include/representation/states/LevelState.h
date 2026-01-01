#pragma once
#include "State.h"
#include "logic/World.h"
#include "logic/Camera.h"
#include "representation/ConcreteFactory.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace pacman::representation {

/**
 * @brief LevelState - actieve gameplay state
 *
 * === VERANTWOORDELIJKHEDEN ===
 * - World/Factory/Camera lifecycle
 * - Map loading
 * - Input → World translation (arrow keys → PacMan direction)
 * - UI rendering (score, lives, timer, controls)
 * - Game state checks (level complete, game over)
 * - "READY" screen (2 sec delay bij start/death)
 *
 * === OWNERSHIP ===
 * ```
 * LevelState owns:
 *   ├─ camera (shared_ptr, shared met World en Views)
 *   ├─ factory (shared_ptr, moet outlive World)
 *   └─ world (unique_ptr)
 * ```
 *
 * === DESTRUCTION ORDER (CRITICAL) ===
 * ~LevelState():
 * 1. factory.reset()  → destroy Views (detach observers)
 * 2. world.reset()    → destroy Models
 *
 * Omgekeerde volgorde = crash (dangling observer pointers).
 *
 * === LETTERBOXING UI ===
 * If viewport heeft letterbox bars:
 * - Left sidebar: score, lives, level
 * - Right sidebar: timer, controls, point values
 * - Viewport center: game world
 *
 * Else (small window):
 * - Compact UI: top-left corner
 *
 * === "READY" MECHANIC ===
 * Toont "READY" text voor 2 sec:
 * - Bij level start
 * - Na PacMan death (na death animation)
 *
 * Tijdens ready: World.update(0.0f) → geen movement.
 */
class LevelState : public State {
public:
    /**
     * @brief Constructor
     * @param level Level nummer (1-based)
     * @param isTutorial true = tutorial map, false = normal map
     */
    LevelState(int level = 1, bool isTutorial = false);

    /**
     * @brief Destructor - CRITICAL ORDER
     *
     * Expliciet: factory.reset() VOOR world.reset().
     */
    ~LevelState() override;

    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    // Core game objects (ownership)
    std::shared_ptr<pacman::Camera> camera;
    std::shared_ptr<ConcreteFactory> factory;
    std::unique_ptr<pacman::World> world;

    int currentLevel;
    float elapsedTime = 0.0f;
    bool hasLetterboxing = false;  // UI layout flag

    sf::Font font;

    // Left sidebar UI
    sf::Text scoreTitle;
    sf::Text scoreText;
    sf::Text livesTitle;
    sf::Text livesText;
    sf::Text levelTitle;
    sf::Text levelText;

    // Right sidebar UI
    sf::Text timerTitle;
    sf::Text timerText;
    sf::Text controlsTitle;
    sf::Text controlKeysText;
    sf::Text controlActionsText;
    sf::Text pointsTitle;
    sf::Text pointsLabelsText;
    sf::Text pointsValuesText;

    // "READY" screen state
    bool showingReady = true;
    float readyTimer = 0.0f;
    const float READY_DURATION = 2.0f;
    sf::Text readyText;

    bool wasPlayingDeathAnimation = false;

    bool tutorialMode;

    void loadFont();

    /**
     * @brief Setup UI elements
     *
     * Berekent posities op basis van viewport offsets.
     * Letterbox check: viewportOffsetX > 10.0f
     */
    void setupUI();

    /**
     * @brief Update UI text (score, lives, level, timer)
     *
     * Roep elke frame aan in update().
     * Formats timer als MM:SS.
     */
    void updateUI();

    /**
     * @brief Verwerk arrow key input → PacMan direction
     *
     * Alleen tijdens !showingReady.
     * Direct call naar pacman->setDirection().
     */
    void handlePlayerInput();

    /**
     * @brief Check game state en trigger transitions
     *
     * Checks:
     * - world.isGameOver() → SWITCH VictoryState(false)
     * - world.isLevelComplete() → SWITCH VictoryState(true)
     */
    void checkGameState();
};

} // namespace pacman::representation