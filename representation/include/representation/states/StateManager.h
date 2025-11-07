#pragma once
#include "State.h"
#include <stack>
#include <memory>

namespace pacman::representation {

class StateManager {
public:
    StateManager() = default;
    ~StateManager() = default;
    
    // State management
    void pushState(std::unique_ptr<State> state);
    void popState();
    void switchState(std::unique_ptr<State> state);
    
    // Game loop methods
    void handleInput(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
    // Query
    bool isEmpty() const { return states.empty(); }
    State* getCurrentState() const;
    
private:
    std::stack<std::unique_ptr<State>> states;
    
    void processStateChanges();
};

} // namespace pacman::representation