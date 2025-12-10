// representation/src/states/StateManager.cpp
#include "representation/states/StateManager.h"

namespace pacman::representation {

void StateManager::pushState(std::unique_ptr<State> state) {
    if (state) {
        state->onEnter();
        states.push(std::move(state));
    }
}

void StateManager::popState() {
    if (!states.empty()) {
        states.top()->onExit();
        states.pop();
    }
}

void StateManager::switchState(std::unique_ptr<State> state) {
    if (!states.empty()) {
        states.top()->onExit();
        states.pop();
    }
    pushState(std::move(state));
}

State* StateManager::getCurrentState() const {
    if (states.empty()) return nullptr;
    return states.top().get();
}

void StateManager::handleInput(const sf::Event& event, sf::RenderWindow& window) {  // ✅ ADD window param
    if (auto* state = getCurrentState()) {
        state->handleInput(event, window);  // ✅ PASS window
        processStateChanges();
    }
}

void StateManager::update(float deltaTime) {
    if (auto* state = getCurrentState()) {
        state->update(deltaTime);
        processStateChanges();
    }
}

void StateManager::render(sf::RenderWindow& window) {
    if (auto* state = getCurrentState()) {
        state->render(window);
    }
}

void StateManager::processStateChanges() {
    auto* state = getCurrentState();
    if (!state || !state->isFinished()) return;

    auto action = state->getAction();
    auto nextState = state->getNextState();

    switch (action) {
    case StateAction::PUSH:
        pushState(std::move(nextState));
        break;
    case StateAction::POP:
        popState();
        break;
    case StateAction::SWITCH:
        switchState(std::move(nextState));
        break;
    default:
        break;
    }
}

} // namespace pacman::representation