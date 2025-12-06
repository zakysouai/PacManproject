#include "representation/states/LevelState.h"
#include "representation/states/PausedState.h"
#include "representation/states/VictoryState.h"
#include "logic/utils/Stopwatch.h"
#include "representation/views/DoorView.h"

#include <iostream>
#include <iomanip>
#include <sstream>

namespace pacman::representation {

LevelState::LevelState(int level)
    : currentLevel(level) {
}

void LevelState::onEnter() {
    std::cout << "Entering LevelState (Level " << currentLevel << ")" << std::endl;

    camera = std::make_unique<pacman::Camera>(1000, 600);
    factory = std::make_unique<ConcreteFactory>(*camera);
    world = std::make_unique<pacman::World>(factory.get());

    world->setCamera(camera.get());
    world->loadLevel("../resources/maps/map_big.txt");

    if (world->hasDoorInMap()) {
        auto doorView = std::make_unique<DoorView>(*camera, world->getDoorPosition());
        factory->addView(std::move(doorView));
    }

    loadFont();
    setupUI();

    // ✅ Reset timer
    elapsedTime = 0.0f;
}

void LevelState::loadFont() {
    if (!font.loadFromFile("../resources/fonts/Retro.ttf")) {
        std::cerr << "Warning: Could not load font" << std::endl;
    }
}

// representation/src/states/LevelState.cpp - setupUI()

void LevelState::setupUI() {
    auto cam = camera.get();
    float leftSidebarX = 30.0f;
    float rightSidebarX = cam->getViewportOffsetX() + cam->getViewportWidth() + 30.0f;

    // ===== LEFT SIDEBAR =====

    scoreTitle.setFont(font);
    scoreTitle.setString("SCORE");
    scoreTitle.setCharacterSize(24);
    scoreTitle.setFillColor(sf::Color(255, 215, 0));
    scoreTitle.setPosition(leftSidebarX, 80);

    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(leftSidebarX, 115);

    livesTitle.setFont(font);
    livesTitle.setString("LIVES");
    livesTitle.setCharacterSize(24);
    livesTitle.setFillColor(sf::Color(255, 215, 0));
    livesTitle.setPosition(leftSidebarX, 200);

    livesText.setFont(font);
    livesText.setCharacterSize(36);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(leftSidebarX, 235);

    levelTitle.setFont(font);
    levelTitle.setString("LEVEL");
    levelTitle.setCharacterSize(24);
    levelTitle.setFillColor(sf::Color(255, 215, 0));
    levelTitle.setPosition(leftSidebarX, 310);

    levelText.setFont(font);
    levelText.setCharacterSize(36);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(leftSidebarX, 345);

    // ===== RIGHT SIDEBAR =====

    timerTitle.setFont(font);
    timerTitle.setString("TIME");
    timerTitle.setCharacterSize(24);
    timerTitle.setFillColor(sf::Color(255, 215, 0));
    timerTitle.setPosition(rightSidebarX, 80);

    timerText.setFont(font);
    timerText.setCharacterSize(40);
    timerText.setFillColor(sf::Color::Cyan);
    timerText.setPosition(rightSidebarX, 115);

    controlsTitle.setFont(font);
    controlsTitle.setString("CONTROLS");
    controlsTitle.setCharacterSize(22);
    controlsTitle.setFillColor(sf::Color(255, 215, 0));
    controlsTitle.setPosition(rightSidebarX, 220);

    controlKeysText.setFont(font);
    controlKeysText.setString("Arrow Keys\nESC");
    controlKeysText.setCharacterSize(18);
    controlKeysText.setFillColor(sf::Color(180, 180, 180));
    controlKeysText.setLineSpacing(1.8f);
    controlKeysText.setPosition(rightSidebarX, 255);

    controlActionsText.setFont(font);
    controlActionsText.setString("Move\nPause");
    controlActionsText.setCharacterSize(18);
    controlActionsText.setFillColor(sf::Color::White);
    controlActionsText.setLineSpacing(1.8f);
    controlActionsText.setPosition(rightSidebarX + 120, 255);

    pointsTitle.setFont(font);
    pointsTitle.setString("COLLECT");
    pointsTitle.setCharacterSize(22);
    pointsTitle.setFillColor(sf::Color(255, 215, 0));
    pointsTitle.setPosition(rightSidebarX, 380);

    pointsLabelsText.setFont(font);
    pointsLabelsText.setString("Coins\nFruits\nGhosts");
    pointsLabelsText.setCharacterSize(18);
    pointsLabelsText.setFillColor(sf::Color(180, 180, 180));
    pointsLabelsText.setLineSpacing(1.8f);
    pointsLabelsText.setPosition(rightSidebarX, 415);

    pointsValuesText.setFont(font);
    pointsValuesText.setString("10PTS\n50PTS\n200PTS");
    pointsValuesText.setCharacterSize(18);
    pointsValuesText.setFillColor(sf::Color::White);
    pointsValuesText.setLineSpacing(1.8f);
    pointsValuesText.setPosition(rightSidebarX + 100, 415);
}

void LevelState::onExit() {
    std::cout << "Exiting LevelState" << std::endl;
}

void LevelState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            finish(StateAction::PUSH, std::make_unique<PausedState>());
        }
    }
    handlePlayerInput();
}

void LevelState::handlePlayerInput() {
    auto* pacman = world->getPacMan();
    if (!pacman) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        pacman->setDirection(pacman::Direction::UP);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        pacman->setDirection(pacman::Direction::DOWN);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        pacman->setDirection(pacman::Direction::LEFT);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        pacman->setDirection(pacman::Direction::RIGHT);
    }
}

void LevelState::update(float deltaTime) {
    elapsedTime += deltaTime;

    world->update(deltaTime);

    for (const auto& view : factory->getViews()) {
        view->update(deltaTime);
    }

    updateUI();
    checkGameState();
}

void LevelState::updateUI() {
    auto* score = world->getScore();
    auto* pacman = world->getPacMan();

    if (score) {
        scoreText.setString(std::to_string(score->getCurrentScore()));
    }

    if (pacman) {
        livesText.setString(std::to_string(pacman->getLives()));
    }

    levelText.setString(std::to_string(currentLevel));

    // ✅ FORMAT TIMER
    int minutes = static_cast<int>(elapsedTime) / 60;
    int seconds = static_cast<int>(elapsedTime) % 60;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes
        << ":" << std::setfill('0') << std::setw(2) << seconds;
    timerText.setString(oss.str());
}

void LevelState::checkGameState() {
    if (world->isGameOver()) {
        int finalScore = world->getScore()->getCurrentScore();
        finish(StateAction::SWITCH, std::make_unique<VictoryState>(false, finalScore, currentLevel));
    } else if (world->isLevelComplete()) {
        int finalScore = world->getScore()->getCurrentScore();
        finish(StateAction::SWITCH, std::make_unique<VictoryState>(true, finalScore, currentLevel));
    }
}

void LevelState::render(sf::RenderWindow& window) {
    window.clear(sf::Color(15, 15, 15));

    auto cam = camera.get();

    // ✅ SIDEBAR BACKGROUNDS
    if (cam->getViewportOffsetX() > 0) {
        // Left sidebar
        sf::RectangleShape leftBg;
        leftBg.setSize(sf::Vector2f(cam->getViewportOffsetX(), cam->getWindowHeight()));
        leftBg.setFillColor(sf::Color(20, 20, 30));
        leftBg.setPosition(0, 0);
        window.draw(leftBg);

        // Right sidebar
        sf::RectangleShape rightBg;
        rightBg.setSize(sf::Vector2f(cam->getViewportOffsetX(), cam->getWindowHeight()));
        rightBg.setFillColor(sf::Color(20, 20, 30));
        rightBg.setPosition(cam->getViewportOffsetX() + cam->getViewportWidth(), 0);
        window.draw(rightBg);

        // ✅ DECORATIVE BORDERS
        sf::RectangleShape leftLine(sf::Vector2f(3, cam->getWindowHeight()));
        leftLine.setFillColor(sf::Color(255, 215, 0));
        leftLine.setPosition(cam->getViewportOffsetX() - 3, 0);
        window.draw(leftLine);

        sf::RectangleShape rightLine(sf::Vector2f(3, cam->getWindowHeight()));
        rightLine.setFillColor(sf::Color(255, 215, 0));
        rightLine.setPosition(cam->getViewportOffsetX() + cam->getViewportWidth(), 0);
        window.draw(rightLine);

        // ✅ SECTION DIVIDERS (left sidebar)
        sf::RectangleShape divider1(sf::Vector2f(cam->getViewportOffsetX() - 40, 2));
        divider1.setFillColor(sf::Color(80, 80, 90));
        divider1.setPosition(20, 190);
        window.draw(divider1);

        sf::RectangleShape divider2(sf::Vector2f(cam->getViewportOffsetX() - 40, 2));
        divider2.setFillColor(sf::Color(80, 80, 90));
        divider2.setPosition(20, 300);
        window.draw(divider2);

        // ✅ SECTION DIVIDERS (right sidebar)
        float rightDividerX = cam->getViewportOffsetX() + cam->getViewportWidth() + 20;
        sf::RectangleShape divider3(sf::Vector2f(cam->getViewportOffsetX() - 40, 2));
        divider3.setFillColor(sf::Color(80, 80, 90));
        divider3.setPosition(rightDividerX, 210);
        window.draw(divider3);

        sf::RectangleShape divider4(sf::Vector2f(cam->getViewportOffsetX() - 40, 2));
        divider4.setFillColor(sf::Color(80, 80, 90));
        divider4.setPosition(rightDividerX, 370);
        window.draw(divider4);
    }

    // Draw game entities
    for (const auto& view : factory->getViews()) {
        view->draw(window);
    }

    // ✅ DRAW UI TEXT
    // Left sidebar
    window.draw(scoreTitle);
    window.draw(scoreText);
    window.draw(livesTitle);
    window.draw(livesText);
    window.draw(levelTitle);
    window.draw(levelText);

    // Right sidebar
    window.draw(timerTitle);
    window.draw(timerText);
    window.draw(controlsTitle);
    window.draw(controlKeysText);
    window.draw(controlActionsText);
    window.draw(pointsTitle);
    window.draw(pointsLabelsText);
    window.draw(pointsValuesText);
}

} // namespace pacman::representation