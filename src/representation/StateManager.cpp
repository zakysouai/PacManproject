#include "StateManager.h"

namespace PacMan {

    void StateManager::pushState(std::unique_ptr<State> state) {
        if (!m_states.empty()) {
            m_states.back()->onExit();
        }
        m_states.push_back(std::move(state));
        m_states.back()->onEnter();
    }

    void StateManager::popState() {
        if (!m_states.empty()) {
            m_states.back()->onExit();
            m_states.pop_back();

            if (!m_states.empty()) {
                m_states.back()->onEnter();
            }
        }
    }

    void StateManager::changeState(std::unique_ptr<State> state) {
        if (!m_states.empty()) {
            m_states.back()->onExit();
            m_states.pop_back();
        }
        m_states.push_back(std::move(state));
        m_states.back()->onEnter();
    }

    void StateManager::handleInput(const sf::Event& event) {
        if (!m_states.empty()) {
            m_states.back()->handleInput(event, *this);
        }
    }

    void StateManager::update(float deltaTime) {
        if (!m_states.empty()) {
            m_states.back()->update(deltaTime, *this);
        }
    }

    void StateManager::render(sf::RenderWindow& window) {
        if (!m_states.empty()) {
            m_states.back()->render(window);
        }
    }

} // namespace PacMan