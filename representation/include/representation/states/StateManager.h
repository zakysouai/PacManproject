// representation/include/representation/states/StateManager.h
#pragma once
#include "State.h"
#include <stack>
#include <memory>

namespace pacman::representation {

class StateManager {
public:
    StateManager() = default;
    ~StateManager() = default;

    void pushState(std::unique_ptr<State> state);
    void popState();
    void switchState(std::unique_ptr<State> state);

    void handleInput(const sf::Event& event, sf::RenderWindow& window);  // ✅ ADD window param
    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    bool isEmpty() const { return states.empty(); }
    State* getCurrentState() const;
    
private:
    std::stack<std::unique_ptr<State>> states;
    
    void processStateChanges();
};

} // namespace pacman::representation