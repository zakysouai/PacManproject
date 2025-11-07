#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace pacman::representation {

class StateManager;  // Forward declaration

enum class StateAction {
    NONE,
    PUSH,     // Add new state on top
    POP,      // Remove current state
    SWITCH    // Replace current state
};

class State {
public:
    virtual ~State() = default;
    
    // Core state methods
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    
    // State transitions
    bool isFinished() const { return finished; }
    StateAction getAction() const { return action; }
    std::unique_ptr<State> getNextState() { return std::move(nextState); }
    
protected:
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