#pragma once
#include "State.h"
#include <stack>
#include <memory>

namespace pacman::representation {

/**
 * @brief StateManager - FSM context (State pattern)
 *
 * === VERANTWOORDELIJKHEDEN ===
 * - State stack management (push/pop/switch)
 * - Delegate input/update/render naar active state
 * - Process state transitions
 *
 * === STATE STACK ===
 * Stack van unique_ptr<State>:
 * - Top = currently active state
 * - Lower states = paused/suspended
 *
 * Voorbeeld stack:
 * ```
 * [PausedState]   ← top (active)
 * [LevelState]    ← suspended (maar nog in memory)
 * [MenuState]     ← bottom
 * ```
 *
 * === TRANSITION PROCESSING ===
 * Gebeurt NA handleInput/update (niet tijdens):
 * 1. Check if current state is finished
 * 2. Execute action (push/pop/switch)
 * 3. Call onExit/onEnter hooks
 *
 * Dit voorkomt state being destroyed tijdens eigen update.
 */
class StateManager {
public:
    StateManager() = default;
    ~StateManager() = default;

    /**
     * @brief Voeg state toe aan top van stack
     * @param state unique_ptr naar nieuwe state
     *
     * Roept state->onEnter() aan.
     * Oude top blijft in stack (suspended).
     */
    void pushState(std::unique_ptr<State> state);

    /**
     * @brief Verwijder top state van stack
     *
     * Roept state->onExit() aan.
     * Underlying state wordt weer active.
     */
    void popState();

    /**
     * @brief Vervang top state
     * @param state unique_ptr naar nieuwe state
     *
     * Equivalent aan: popState() + pushState().
     * Gebruikt voor level complete, game over, etc.
     */
    void switchState(std::unique_ptr<State> state);

    /**
     * @brief Delegate input naar active state
     * @param event SFML event
     * @param window Window reference
     *
     * Na handleInput: process transitions.
     */
    void handleInput(const sf::Event& event, sf::RenderWindow& window);

    /**
     * @brief Delegate update naar active state
     * @param deltaTime Tijd sinds laatste frame
     *
     * Na update: process transitions.
     */
    void update(float deltaTime);

    /**
     * @brief Delegate render naar active state
     * @param window SFML window
     */
    void render(sf::RenderWindow& window);

    bool isEmpty() const { return states.empty(); }
    State* getCurrentState() const;

private:
    std::stack<std::unique_ptr<State>> states;  // State stack (ownership)

    /**
     * @brief Verwerk pending state transitions
     *
     * Roept PUSH/POP/SWITCH uit op basis van current state's action.
     * Called na handleInput en update.
     */
    void processStateChanges();
};

} // namespace pacman::representation