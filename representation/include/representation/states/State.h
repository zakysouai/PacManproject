#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace pacman::representation {

class StateManager;

/**
 * @brief State actions voor transitions
 *
 * - NONE: Geen transitie
 * - PUSH: Voeg nieuwe state toe (pause, submenu)
 * - POP: Verwijder huidige state (resume, back)
 * - SWITCH: Vervang huidige state (level complete, game over)
 */
enum class StateAction {
    NONE,
    PUSH,
    POP,
    SWITCH
};

/**
 * @brief Abstracte State base - FSM (Finite State Machine) pattern
 *
 * === STATE MACHINE DESIGN ===
 * States zijn schermen/modes in game:
 * - MenuState: Hoofdmenu met highscores
 * - LevelState: Actieve gameplay
 * - PausedState: Paused overlay
 * - VictoryState: Win/lose screen
 * - TutorialIntroState: Tutorial instructies
 *
 * === STATE STACK ===
 * StateManager gebruikt stack:
 * - Top van stack = active state
 * - PUSH: nieuwe state bovenop (pause over level)
 * - POP: verwijder top (resume naar level)
 * - SWITCH: pop + push (level complete → victory)
 *
 * === TRANSITION FLOW ===
 * 1. State roept finish(action, nextState) aan
 * 2. StateManager detecteert finished flag
 * 3. StateManager voert action uit (push/pop/switch)
 * 4. onExit() van oude state
 * 5. onEnter() van nieuwe state
 *
 * === LIFECYCLE ===
 * 1. Construction: State object gemaakt
 * 2. onEnter(): Initialisatie bij activation
 * 3. handleInput/update/render: Main loop
 * 4. finish(): Trigger transition
 * 5. onExit(): Cleanup bij deactivation
 * 6. Destruction: State object destroyed
 */
class State {
public:
    virtual ~State() = default;

    /**
     * @brief Called wanneer state active wordt
     *
     * Gebruik voor:
     * - Reset timers
     * - Load resources (als niet in constructor)
     * - Print debug info
     */
    virtual void onEnter() = 0;

    /**
     * @brief Called wanneer state inactive wordt
     *
     * Gebruik voor:
     * - Save state
     * - Cleanup resources
     * - Print debug info
     */
    virtual void onExit() = 0;

    /**
     * @brief Verwerk input events
     * @param event SFML event (keyboard, mouse, window)
     * @param window Reference naar window (voor mouse coords conversie)
     *
     * Typisch:
     * - Keyboard: menu navigation, pause, player movement
     * - Mouse: button clicks
     */
    virtual void handleInput(const sf::Event& event, sf::RenderWindow& window) = 0;

    /**
     * @brief Update state logica
     * @param deltaTime Tijd sinds laatste frame
     *
     * Typisch:
     * - Animatie timers
     * - World update (in LevelState)
     * - Blink effects
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Render state naar window
     * @param window SFML window
     *
     * BELANGRIJK: States zijn verantwoordelijk voor eigen clear/view setup.
     * LevelState: render game world
     * MenuState: render menu UI
     * PausedState: GEEN clear (render over LevelState)
     */
    virtual void render(sf::RenderWindow& window) = 0;

    // Transition getters (voor StateManager)
    bool isFinished() const { return finished; }
    StateAction getAction() const { return action; }
    std::unique_ptr<State> getNextState() { return std::move(nextState); }

protected:
    /**
     * @brief Trigger state transition
     * @param action Type van transition (PUSH/POP/SWITCH)
     * @param next Optional: nieuwe state (voor PUSH/SWITCH)
     *
     * Voorbeelden:
     * ```cpp
     * // Pause game (push PausedState over LevelState):
     * finish(StateAction::PUSH, std::make_unique<PausedState>());
     *
     * // Resume game (pop PausedState):
     * finish(StateAction::POP);
     *
     * // Level complete (switch naar VictoryState):
     * finish(StateAction::SWITCH, std::make_unique<VictoryState>(true, score, level));
     * ```
     */
    void finish(StateAction action, std::unique_ptr<State> next = nullptr) {
        finished = true;
        this->action = action;
        nextState = std::move(next);
    }
    
    bool finished = false;
    StateAction action = StateAction::NONE;
    std::unique_ptr<State> nextState;
};

} // namespace pacman::representation