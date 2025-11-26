#include "representation/states/PausedState.h"
#include "representation/states/MenuState.h"
#include <iostream>

namespace pacman::representation {

PausedState::PausedState() {
    loadFont();
    setupTexts();
}

void PausedState::onEnter() {
    std::cout << "Game Paused" << std::endl;
}

void PausedState::onExit() {
    std::cout << "Resuming Game" << std::endl;
}

void PausedState::loadFont() {
    if (!font.loadFromFile("../resources/fonts/arial.ttf")) {
        std::cerr << "Warning: Could not load font" << std::endl;
    }
}

void PausedState::setupTexts() {
    // Semi-transparent overlay
    overlay.setSize(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    
    pausedText.setFont(font);
    pausedText.setString("PAUSED");
    pausedText.setCharacterSize(72);
    pausedText.setFillColor(sf::Color::Yellow);
    pausedText.setPosition(270, 200);
    
    instructionsText.setFont(font);
    instructionsText.setString("Press ESC to Resume\nPress M for Menu");
    instructionsText.setCharacterSize(24);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setPosition(260, 350);
}

void PausedState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            // Resume (pop paused state)
            finish(StateAction::POP);
        } else if (event.key.code == sf::Keyboard::M) {
            // Go to menu (switch to menu, clearing state stack)
            finish(StateAction::SWITCH, std::make_unique<MenuState>());
        }
    }
}

void PausedState::update(float deltaTime) {
    // Nothing to update while paused
}

void PausedState::render(sf::RenderWindow& window) {
    // Don't clear - we want to see the game underneath
    window.draw(overlay);
    window.draw(pausedText);
    window.draw(instructionsText);
}

} // namespace pacman::representation