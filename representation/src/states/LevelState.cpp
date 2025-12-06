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
    if (!font.loadFromFile("../resources/fonts/arial.ttf")) {
        std::cerr << "Warning: Could not load font" << std::endl;
    }
}

void LevelState::setupUI() {
    auto cam = camera.get();
    float leftSidebarX = 20.0f;
    float rightSidebarX = cam->getViewportOffsetX() + cam->getViewportWidth() + 20.0f;

    // ✅ LEFT SIDEBAR - SCORE & LIVES
    scoreText.setFont(font);
    scoreText.setCharacterSize(28);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(leftSidebarX, 100);

    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(leftSidebarX, 200);

    levelText.setFont(font);
    levelText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    levelText.setPosition(leftSidebarX, 250);

    // ✅ RIGHT SIDEBAR - TIMER & CONTROLS
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::Cyan);
    timerText.setPosition(rightSidebarX, 100);

    controlsTitle.setFont(font);
    controlsTitle.setString("CONTROLS");
    controlsTitle.setCharacterSize(22);
    controlsTitle.setFillColor(sf::Color::Yellow);
    controlsTitle.setPosition(rightSidebarX, 200);

    controlsText.setFont(font);
    controlsText.setString(
        "Arrow Keys:\n  Move\n\n"
        "ESC:\n  Pause\n\n"
        "Collect:\n"
        "  Coins: 10pts\n"
        "  Fruits: 50pts\n"
        "  Ghosts: 200pts"
    );
    controlsText.setCharacterSize(16);
    controlsText.setFillColor(sf::Color(200, 200, 200));
    controlsText.setPosition(rightSidebarX, 240);
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
        scoreText.setString("SCORE\n" + std::to_string(score->getCurrentScore()));
    }

    if (pacman) {
        livesText.setString("LIVES: " + std::to_string(pacman->getLives()));
    }

    levelText.setString("LEVEL: " + std::to_string(currentLevel));

    // ✅ FORMAT TIMER
    int minutes = static_cast<int>(elapsedTime) / 60;
    int seconds = static_cast<int>(elapsedTime) % 60;
    std::ostringstream oss;
    oss << "TIME\n" << std::setfill('0') << std::setw(2) << minutes
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
    window.clear(sf::Color(15, 15, 15));  // Dark gray background

    auto cam = camera.get();

    // ✅ DECORATIEVE SIDEBAR ACHTERGRONDEN
    if (cam->getViewportOffsetX() > 0) {
        // Left sidebar background
        sf::RectangleShape leftBg;
        leftBg.setSize(sf::Vector2f(cam->getViewportOffsetX(), cam->getWindowHeight()));
        leftBg.setFillColor(sf::Color(25, 25, 35));
        leftBg.setPosition(0, 0);
        window.draw(leftBg);

        // Right sidebar background
        sf::RectangleShape rightBg;
        rightBg.setSize(sf::Vector2f(cam->getViewportOffsetX(), cam->getWindowHeight()));
        rightBg.setFillColor(sf::Color(25, 25, 35));
        rightBg.setPosition(cam->getViewportOffsetX() + cam->getViewportWidth(), 0);
        window.draw(rightBg);

        // ✅ DECORATIEVE LIJNEN
        sf::RectangleShape leftLine(sf::Vector2f(3, cam->getWindowHeight()));
        leftLine.setFillColor(sf::Color::Yellow);
        leftLine.setPosition(cam->getViewportOffsetX() - 3, 0);
        window.draw(leftLine);

        sf::RectangleShape rightLine(sf::Vector2f(3, cam->getWindowHeight()));
        rightLine.setFillColor(sf::Color::Yellow);
        rightLine.setPosition(cam->getViewportOffsetX() + cam->getViewportWidth(), 0);
        window.draw(rightLine);
    }

    // Draw game entities
    for (const auto& view : factory->getViews()) {
        view->draw(window);
    }

    // ✅ DRAW SIDEBAR UI
    window.draw(scoreText);
    window.draw(livesText);
    window.draw(levelText);
    window.draw(timerText);
    window.draw(controlsTitle);
    window.draw(controlsText);
}

} // namespace pacman::representation