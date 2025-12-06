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
    if (!font.loadFromFile("../resources/fonts/Retro.ttf")) {
        std::cerr << "Warning: Could not load font, using default" << std::endl;
    }
}

void MenuState::setupTexts() {
    float windowWidth = 1000.0f;  // Van Game.h WINDOW_WIDTH
    float windowHeight = 600.0f;  // Van Game.h WINDOW_HEIGHT
    float centerX = windowWidth / 2.0f;

    // ✅ TITLE - GECENTREERD
    titleText.setFont(font);
    titleText.setString("PAC-MAN");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Yellow);

    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2.0f, titleBounds.height / 2.0f);
    titleText.setPosition(centerX, 100);

    // ✅ PLAY BUTTON - GECENTREERD
    playButton.setSize(sf::Vector2f(250, 70));
    playButton.setFillColor(sf::Color(30, 30, 200));
    playButton.setOrigin(125, 35);  // Width/2, Height/2
    playButton.setPosition(centerX, 250);

    playButtonText.setFont(font);
    playButtonText.setString("PLAY");
    playButtonText.setCharacterSize(40);
    playButtonText.setFillColor(sf::Color::White);

    sf::FloatRect playBounds = playButtonText.getLocalBounds();
    playButtonText.setOrigin(playBounds.width / 2.0f, playBounds.height / 2.0f);
    playButtonText.setPosition(centerX, 250);

    // ✅ HIGH SCORES TITLE - GECENTREERD
    highScoresTitle.setFont(font);
    highScoresTitle.setString("HIGH SCORES");
    highScoresTitle.setCharacterSize(32);
    highScoresTitle.setFillColor(sf::Color(255, 215, 0));  // Goud

    sf::FloatRect scoreTitleBounds = highScoresTitle.getLocalBounds();
    highScoresTitle.setOrigin(scoreTitleBounds.width / 2.0f, 0);
    highScoresTitle.setPosition(centerX, 370);
}


void MenuState::loadHighScores() {
    pacman::Score tempScore;  // ✅ Temporary om te laden
    auto scores = tempScore.getHighScores();

    float centerX = 500.0f;
    float yPos = 420;

    highScoreTexts.clear();

    for (size_t i = 0; i < scores.size() && i < 5; ++i) {
        sf::Text text;
        text.setFont(font);

        std::string displayText = std::to_string(i + 1) + ".  " +
                                  scores[i].name + "  " +
                                  std::to_string(scores[i].score);

        text.setString(displayText);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2.0f, 0);
        text.setPosition(centerX, yPos);

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
    if (playButtonHovered) {
        playButton.setFillColor(sf::Color(60, 60, 255));

        // ✅ SCALE EFFECT
        float centerX = playButton.getPosition().x;
        float centerY = playButton.getPosition().y;
        playButton.setOrigin(125, 35);
        playButton.setScale(1.05f, 1.05f);
        playButton.setPosition(centerX, centerY);

        playButtonText.setScale(1.05f, 1.05f);
    } else {
        playButton.setFillColor(sf::Color(30, 30, 200));
        playButton.setScale(1.0f, 1.0f);
        playButtonText.setScale(1.0f, 1.0f);
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