// representation/src/states/MenuState.cpp
#include "representation/states/MenuState.h"
#include "representation/states/LevelState.h"
#include <iostream>
#include <representation/states/TutorialIntroState.h>

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
    if (!font.loadFromFile("../resources/fonts/Retro.ttf")) {
        std::cerr << "Warning: Could not load font, using default" << std::endl;
    }
}

void MenuState::setupTexts() {
    float centerX = 500.0f;  // ✅ Fixed coordinate system

    titleText.setFont(font);
    titleText.setString("PAC-MAN");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2.0f, titleBounds.height / 2.0f);
    titleText.setPosition(centerX, 100);

    playButton.setSize(sf::Vector2f(250, 70));
    playButton.setFillColor(sf::Color(30, 30, 200));
    playButton.setOrigin(125, 35);
    playButton.setPosition(centerX, 220);

    playButtonText.setFont(font);
    playButtonText.setString("PLAY");
    playButtonText.setCharacterSize(40);
    playButtonText.setFillColor(sf::Color::White);
    sf::FloatRect playBounds = playButtonText.getLocalBounds();
    playButtonText.setOrigin(playBounds.width / 2.0f, playBounds.height / 2.0f);
    playButtonText.setPosition(centerX, 220);

    tutorialButton.setSize(sf::Vector2f(250, 70));
    tutorialButton.setFillColor(sf::Color(30, 30, 200));
    tutorialButton.setOrigin(125, 35);
    tutorialButton.setPosition(centerX, 310);

    tutorialButtonText.setFont(font);
    tutorialButtonText.setString("TUTORIAL");
    tutorialButtonText.setCharacterSize(40);
    tutorialButtonText.setFillColor(sf::Color::White);
    sf::FloatRect tutBounds = tutorialButtonText.getLocalBounds();
    tutorialButtonText.setOrigin(tutBounds.width / 2.0f, tutBounds.height / 2.0f);
    tutorialButtonText.setPosition(centerX, 310);

    highScoresTitle.setFont(font);
    highScoresTitle.setString("HIGH SCORES");
    highScoresTitle.setCharacterSize(32);
    highScoresTitle.setFillColor(sf::Color(255, 215, 0));
    sf::FloatRect scoreTitleBounds = highScoresTitle.getLocalBounds();
    highScoresTitle.setOrigin(scoreTitleBounds.width / 2.0f, 0);
    highScoresTitle.setPosition(centerX, 400);
}

void MenuState::loadHighScores() {
    pacman::Score tempScore;
    auto scores = tempScore.getHighScores();

    float centerX = 500.0f;
    float yPos = 450;

    highScoreTexts.clear();

    for (size_t i = 0; i < scores.size() && i < 5; ++i) {
        sf::Text text;
        text.setFont(font);

        std::string rank = std::to_string(i + 1) + ".";
        std::string name = scores[i].name;
        std::string score = std::to_string(scores[i].score);

        while (name.length() < 12) name += " ";
        while (score.length() < 6) score = " " + score;

        std::string displayText = rank + "  " + name + "  " + score;

        text.setString(displayText);
        text.setCharacterSize(22);
        text.setFillColor(sf::Color::White);

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2.0f, 0);
        text.setPosition(centerX, yPos);

        yPos += 28;
        highScoreTexts.push_back(text);
    }
}

sf::Vector2f MenuState::mapPixelToCoords(const sf::RenderWindow& window, sf::Vector2i pixel) {
    return window.mapPixelToCoords(pixel);
}

void MenuState::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);  // ✅ Use window reference

            if (playButton.getGlobalBounds().contains(mousePos)) {
                finish(StateAction::PUSH, std::make_unique<LevelState>(1, false));
            } else if (tutorialButton.getGlobalBounds().contains(mousePos)) {
                finish(StateAction::PUSH, std::make_unique<TutorialIntroState>());
            }
        }
    }

    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i pixelPos(event.mouseMove.x, event.mouseMove.y);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);  // ✅ Use window reference
        playButtonHovered = playButton.getGlobalBounds().contains(mousePos);
        tutorialButtonHovered = tutorialButton.getGlobalBounds().contains(mousePos);
    }
}

void MenuState::update(float deltaTime) {
    if (playButtonHovered) {
        playButton.setFillColor(sf::Color(60, 60, 255));
        playButton.setScale(1.05f, 1.05f);
        playButtonText.setScale(1.05f, 1.05f);
    } else {
        playButton.setFillColor(sf::Color(30, 30, 200));
        playButton.setScale(1.0f, 1.0f);
        playButtonText.setScale(1.0f, 1.0f);
    }

    if (tutorialButtonHovered) {
        tutorialButton.setFillColor(sf::Color(60, 60, 255));
        tutorialButton.setScale(1.05f, 1.05f);
        tutorialButtonText.setScale(1.05f, 1.05f);
    } else {
        tutorialButton.setFillColor(sf::Color(30, 30, 200));
        tutorialButton.setScale(1.0f, 1.0f);
        tutorialButtonText.setScale(1.0f, 1.0f);
    }
}

void MenuState::render(sf::RenderWindow& window) {
    sf::View view(sf::FloatRect(0, 0, 1000, 600));
    window.setView(view);

    window.clear(sf::Color::Black);
    window.draw(titleText);
    window.draw(playButton);
    window.draw(playButtonText);
    window.draw(tutorialButton);
    window.draw(tutorialButtonText);
    window.draw(highScoresTitle);
    
    for (auto& text : highScoreTexts) {
        window.draw(text);
    }
}

bool MenuState::isMouseOverButton(const sf::Vector2f& mousePos) const {
    return playButton.getGlobalBounds().contains(mousePos);
}

} // namespace pacman::representation