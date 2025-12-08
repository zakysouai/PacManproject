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
    if (!font.loadFromFile("../resources/fonts/Retro.ttf")) {
        std::cerr << "Warning: Could not load font, using default" << std::endl;
    }
}

void MenuState::setupTexts() {
    float centerX = 500.0f;

    // Title (blijft hetzelfde)
    titleText.setFont(font);
    titleText.setString("PAC-MAN");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2.0f, titleBounds.height / 2.0f);
    titleText.setPosition(centerX, 100);

    // ✅ PLAY BUTTON (verschuif omhoog)
    playButton.setSize(sf::Vector2f(250, 70));
    playButton.setFillColor(sf::Color(30, 30, 200));
    playButton.setOrigin(125, 35);
    playButton.setPosition(centerX, 220);  // Was 250

    playButtonText.setFont(font);
    playButtonText.setString("PLAY");
    playButtonText.setCharacterSize(40);
    playButtonText.setFillColor(sf::Color::White);
    sf::FloatRect playBounds = playButtonText.getLocalBounds();
    playButtonText.setOrigin(playBounds.width / 2.0f, playBounds.height / 2.0f);
    playButtonText.setPosition(centerX, 220);

    // ✅ TUTORIAL BUTTON (nieuw)
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

    // High scores title (verschuif omlaag)
    highScoresTitle.setFont(font);
    highScoresTitle.setString("HIGH SCORES");
    highScoresTitle.setCharacterSize(32);
    highScoresTitle.setFillColor(sf::Color(255, 215, 0));
    sf::FloatRect scoreTitleBounds = highScoresTitle.getLocalBounds();
    highScoresTitle.setOrigin(scoreTitleBounds.width / 2.0f, 0);
    highScoresTitle.setPosition(centerX, 400);  // Was 370
}

void MenuState::loadHighScores() {
    pacman::Score tempScore;
    auto scores = tempScore.getHighScores();

    float centerX = 500.0f;
    float yPos = 450;  // Was 420

    highScoreTexts.clear();

    for (size_t i = 0; i < scores.size() && i < 5; ++i) {
        sf::Text text;
        text.setFont(font);

        // ✅ PADDING VOOR UITLIJNING
        std::string rank = std::to_string(i + 1) + ".";
        std::string name = scores[i].name;
        std::string score = std::to_string(scores[i].score);

        // Pad name to 12 chars, score to 6 chars
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

void MenuState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

            if (playButton.getGlobalBounds().contains(mousePos)) {
                // ✅ PLAY = direct naar map_big.txt (level 1, geen tutorial)
                finish(StateAction::PUSH, std::make_unique<LevelState>(1, false));
            } else if (tutorialButton.getGlobalBounds().contains(mousePos)) {
                // ✅ TUTORIAL = naar tutorial intro
                finish(StateAction::PUSH, std::make_unique<TutorialIntroState>());
            }
        }
    }

    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
        playButtonHovered = playButton.getGlobalBounds().contains(mousePos);
        tutorialButtonHovered = tutorialButton.getGlobalBounds().contains(mousePos);
    }
}

void MenuState::update(float deltaTime) {
    // Play button hover
    if (playButtonHovered) {
        playButton.setFillColor(sf::Color(60, 60, 255));
        playButton.setScale(1.05f, 1.05f);
        playButtonText.setScale(1.05f, 1.05f);
    } else {
        playButton.setFillColor(sf::Color(30, 30, 200));
        playButton.setScale(1.0f, 1.0f);
        playButtonText.setScale(1.0f, 1.0f);
    }

    // Tutorial button hover
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