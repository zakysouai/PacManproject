#include "representation/states/MenuState.h"
#include "representation/states/LevelState.h"
#include <iostream>

namespace pacman::representation {

MenuState::MenuState() {
    loadFont();
    setupTexts();
    loadHighScores();
}

void MenuState::onEnter() {
    std::cout << "Entering MenuState" << std::endl;
}

void MenuState::onExit() {
    std::cout << "Exiting MenuState" << std::endl;
}

void MenuState::loadFont() {
    // Try to load a font (you'll need to add a font file to resources/)
    if (!font.loadFromFile("../resources/fonts/arial.ttf")) {
        std::cerr << "Warning: Could not load font, using default" << std::endl;
    }
}

void MenuState::setupTexts() {
    // Title
    titleText.setFont(font);
    titleText.setString("PAC-MAN");
    titleText.setCharacterSize(72);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition(250, 50);
    
    // Play button
    playButton.setSize(sf::Vector2f(200, 60));
    playButton.setPosition(300, 250);
    playButton.setFillColor(sf::Color(0, 100, 200));

    // TODO: zorg er voor dat de positie van de play button niet hardcoded is SO naar eray
    playButtonText.setFont(font);
    playButtonText.setString("PLAY");
    playButtonText.setCharacterSize(36);
    playButtonText.setFillColor(sf::Color::White);
    playButtonText.setPosition(350, 260);
    
    // High scores title
    highScoresTitle.setFont(font);
    highScoresTitle.setString("HIGH SCORES");
    highScoresTitle.setCharacterSize(32);
    highScoresTitle.setFillColor(sf::Color::White);
    highScoresTitle.setPosition(280, 350);
}

void MenuState::loadHighScores() {
    // TODO: Load from Score class
    std::vector<int> scores = {5000, 4000, 3000, 2000, 1000};
    
    float yPos = 400;
    for (size_t i = 0; i < scores.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(i + 1) + ".  " + std::to_string(scores[i]));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(320, yPos);
        yPos += 30;
        highScoreTexts.push_back(text);
    }
}

void MenuState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (isMouseOverButton(mousePos)) {
                // Transition to LevelState
                finish(StateAction::PUSH, std::make_unique<LevelState>(1));
            }
        }
    }
    
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
        playButtonHovered = isMouseOverButton(mousePos);
    }
}

void MenuState::update(float deltaTime) {
    // Update button color based on hover
    if (playButtonHovered) {
        playButton.setFillColor(sf::Color(0, 150, 250));
    } else {
        playButton.setFillColor(sf::Color(0, 100, 200));
    }
}

void MenuState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    window.draw(titleText);
    window.draw(playButton);
    window.draw(playButtonText);
    window.draw(highScoresTitle);
    
    for (auto& text : highScoreTexts) {
        window.draw(text);
    }
}

bool MenuState::isMouseOverButton(const sf::Vector2f& mousePos) const {
    return playButton.getGlobalBounds().contains(mousePos);
}

} // namespace pacman::representation