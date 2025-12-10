// representation/src/states/PausedState.cpp
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
    if (!font.loadFromFile("../resources/fonts/Retro.ttf")) {
        std::cerr << "Warning: Could not load font" << std::endl;
    }
}

void PausedState::setupTexts() {
    float windowWidth = 1000.0f;
    float windowHeight = 600.0f;
    float centerX = windowWidth / 2.0f;
    float centerY = windowHeight / 2.0f;

    // ✅ OVERLAY - VOLLEDIG SCHERM
    overlay.setSize(sf::Vector2f(windowWidth, windowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));  // Semi-transparant zwart
    overlay.setPosition(0, 0);

    // ✅ PAUSED TEXT - GECENTREERD
    pausedText.setFont(font);
    pausedText.setString("PAUSED");
    pausedText.setCharacterSize(80);
    pausedText.setFillColor(sf::Color::Yellow);

    sf::FloatRect pausedBounds = pausedText.getLocalBounds();
    pausedText.setOrigin(pausedBounds.width / 2.0f, pausedBounds.height / 2.0f);
    pausedText.setPosition(centerX, centerY - 80);

    // ✅ INSTRUCTIONS TEXT - GECENTREERD
    instructionsText.setFont(font);
    instructionsText.setString("Press ESC to Resume\nPress M for Menu");
    instructionsText.setCharacterSize(28);
    instructionsText.setFillColor(sf::Color::White);

    sf::FloatRect instrBounds = instructionsText.getLocalBounds();
    instructionsText.setOrigin(instrBounds.width / 2.0f, 0);
    instructionsText.setPosition(centerX, centerY + 40);
}

void PausedState::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            finish(StateAction::POP);
        } else if (event.key.code == sf::Keyboard::M) {
            finish(StateAction::SWITCH, std::make_unique<MenuState>());
        }
    }
}

void PausedState::update(float deltaTime) {
    // Nothing to update
}

void PausedState::render(sf::RenderWindow& window) {
    // Niet clearen - we willen het spel eronder zien
    window.draw(overlay);
    window.draw(pausedText);
    window.draw(instructionsText);
}

} // namespace pacman::representation