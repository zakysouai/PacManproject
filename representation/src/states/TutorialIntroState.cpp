#include "representation/states/TutorialIntroState.h"
#include "representation/states/LevelState.h"
#include <iostream>

namespace pacman::representation {

TutorialIntroState::TutorialIntroState() {
    loadFont();
    setupTexts();
}

void TutorialIntroState::onEnter() {
    std::cout << "Entering Tutorial Intro" << std::endl;
}

void TutorialIntroState::onExit() {
    std::cout << "Starting Tutorial Level" << std::endl;
}

void TutorialIntroState::loadFont() {
    if (!font.loadFromFile("../resources/fonts/Retro.ttf")) {
        std::cerr << "Warning: Could not load font" << std::endl;
    }
}

void TutorialIntroState::setupTexts() {
    float centerX = 500.0f;
    
    // Title
    titleText.setFont(font);
    titleText.setString("TUTORIAL");
    titleText.setCharacterSize(64);
    titleText.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2.0f, titleBounds.height / 2.0f);
    titleText.setPosition(centerX, 80);
    
    // Instructions - BETER GEFORMATEERD
    instructionsText.setFont(font);
    instructionsText.setString(
        "OBJECTIVE:\n"
        "  Collect all coins and fruits\n"
        "  to complete the level.\n\n"
        "AVOID GHOSTS:\n"
        "  If a ghost catches you, you lose a life.\n"
        "  You start with 3 lives.\n\n"
        "POWER-UPS:\n"
        "  Eat fruits to turn ghosts blue.\n"
        "  While blue, you can eat them\n"
        "  for bonus points!"
    );
    instructionsText.setCharacterSize(20);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setLineSpacing(1.3f);
    instructionsText.setPosition(200, 150);  // Links uitgelijnd ipv gecentreerd

    // Controls - APARTE BOX
    controlsText.setFont(font);
    controlsText.setString(
        "CONTROLS:\n"
        "  Arrow Keys - Move\n"
        "  ESC - Pause"
    );
    controlsText.setCharacterSize(20);
    controlsText.setFillColor(sf::Color(180, 180, 180));
    controlsText.setLineSpacing(1.3f);
    controlsText.setPosition(600, 390);

    // Continue prompt
    continueText.setFont(font);
    continueText.setString("Press ENTER to start");
    continueText.setCharacterSize(28);
    continueText.setFillColor(sf::Color::Green);
    sf::FloatRect contBounds = continueText.getLocalBounds();
    continueText.setOrigin(contBounds.width / 2.0f, 0);
    continueText.setPosition(centerX, 520);
}

void TutorialIntroState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return) {
            // ✅ Start tutorial level met map.txt
            auto tutorialLevel = std::make_unique<LevelState>(0, true);  // level 0 = tutorial
            finish(StateAction::SWITCH, std::move(tutorialLevel));
        } else if (event.key.code == sf::Keyboard::Escape) {
            // Terug naar menu
            finish(StateAction::POP);
        }
    }
}

void TutorialIntroState::update(float deltaTime) {
    // Blink effect voor continue text
    static float elapsed = 0.0f;
    elapsed += deltaTime;
    
    int alpha = static_cast<int>(127 + 127 * std::sin(elapsed * 3.0f));
    continueText.setFillColor(sf::Color(0, 255, 0, alpha));
}

void TutorialIntroState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    window.draw(titleText);
    window.draw(instructionsText);
    window.draw(controlsText);
    window.draw(continueText);
}

} // namespace pacman::representation