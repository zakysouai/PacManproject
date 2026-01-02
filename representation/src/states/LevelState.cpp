#include "representation/states/LevelState.h"
#include "representation/states/PausedState.h"
#include "representation/states/VictoryState.h"
#include "logic/utils/Stopwatch.h"
#include "representation/views/DoorView.h"

#include <iostream>
#include <iomanip>
#include <sstream>

namespace pacman::representation {

LevelState::LevelState(int level, bool isTutorial)
    : currentLevel(level), tutorialMode(isTutorial) {
}

LevelState::~LevelState() {
    factory.reset();  //  Destroy views FIRST (detach from models)
    world.reset();    //  Then destroy models
}

void LevelState::onEnter() {

    camera = std::make_shared<pacman::Camera>(1000, 600);
    factory = std::make_shared<ConcreteFactory>(camera);
    world = std::make_unique<pacman::World>(*factory, camera, currentLevel);  // ✅ PASS LEVEL

    std::string mapFile;
    if (tutorialMode) {
        mapFile = "../resources/maps/map.txt";
    } else {
        mapFile = "../resources/maps/map_big.txt";
    }

    world->loadLevel(mapFile);

    if (world->hasDoorInMap()) {
        auto doorView = std::make_unique<DoorView>(camera, world->getDoorPosition());
        factory->addView(std::move(doorView));
    }

    loadFont();
    setupUI();

    readyText.setFont(font);
    readyText.setString("READY");
    readyText.setCharacterSize(72);
    readyText.setFillColor(sf::Color::Yellow);

    sf::FloatRect bounds = readyText.getLocalBounds();
    readyText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    float viewportCenterX = camera->getViewportOffsetX() + camera->getViewportWidth() / 2.0f;
    float viewportCenterY = camera->getViewportOffsetY() + camera->getViewportHeight() / 2.0f;
    readyText.setPosition(viewportCenterX, viewportCenterY);

    elapsedTime = 0.0f;
    showingReady = true;
    readyTimer = 0.0f;
    wasPlayingDeathAnimation = false;
}

void LevelState::loadFont() {
    if (!font.loadFromFile("../resources/fonts/Retro.ttf")) {
        std::cerr << "Warning: Could not load font" << std::endl;
    }
}

void LevelState::setupUI() {
    auto cam = camera.get();  // ✅ Direct access to shared_ptr

    hasLetterboxing = cam->getViewportOffsetX() > 10.0f;

    float availableSpace = cam->getViewportOffsetX();
    float leftSidebarX = availableSpace * 0.15f;
    float rightSidebarX = cam->getViewportOffsetX() + cam->getViewportWidth() + availableSpace * 0.15f;

    // Left sidebar setup (unchanged)
    scoreTitle.setFont(font);
    scoreTitle.setString("SCORE");
    scoreTitle.setCharacterSize(24);
    scoreTitle.setFillColor(sf::Color(255, 215, 0));
    scoreTitle.setPosition(leftSidebarX, 100);

    scoreText.setFont(font);
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(leftSidebarX, 135);

    livesTitle.setFont(font);
    livesTitle.setString("LIVES");
    livesTitle.setCharacterSize(24);
    livesTitle.setFillColor(sf::Color(255, 215, 0));
    livesTitle.setPosition(leftSidebarX, 230);

    livesText.setFont(font);
    livesText.setCharacterSize(32);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(leftSidebarX, 265);

    levelTitle.setFont(font);
    levelTitle.setString("LEVEL");
    levelTitle.setCharacterSize(24);
    levelTitle.setFillColor(sf::Color(255, 215, 0));
    levelTitle.setPosition(leftSidebarX, 350);

    levelText.setFont(font);
    levelText.setCharacterSize(32);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(leftSidebarX, 385);

    // Right sidebar setup (unchanged)
    timerTitle.setFont(font);
    timerTitle.setString("TIME");
    timerTitle.setCharacterSize(24);
    timerTitle.setFillColor(sf::Color(255, 215, 0));
    timerTitle.setPosition(rightSidebarX, 100);

    timerText.setFont(font);
    timerText.setCharacterSize(36);
    timerText.setFillColor(sf::Color::Cyan);
    timerText.setPosition(rightSidebarX, 135);

    controlsTitle.setFont(font);
    controlsTitle.setString("CONTROLS");
    controlsTitle.setCharacterSize(22);
    controlsTitle.setFillColor(sf::Color(255, 215, 0));
    controlsTitle.setPosition(rightSidebarX, 240);

    controlKeysText.setFont(font);
    controlKeysText.setString("Arrows\nESC");
    controlKeysText.setCharacterSize(18);
    controlKeysText.setFillColor(sf::Color(180, 180, 180));
    controlKeysText.setLineSpacing(1.6f);
    controlKeysText.setPosition(rightSidebarX, 275);

    controlActionsText.setFont(font);
    controlActionsText.setString("Move\nPause");
    controlActionsText.setCharacterSize(18);
    controlActionsText.setFillColor(sf::Color::White);
    controlActionsText.setLineSpacing(1.6f);
    controlActionsText.setPosition(rightSidebarX + 100, 275);

    pointsTitle.setFont(font);
    pointsTitle.setString("COLLECT");
    pointsTitle.setCharacterSize(22);
    pointsTitle.setFillColor(sf::Color(255, 215, 0));
    pointsTitle.setPosition(rightSidebarX, 400);

    pointsLabelsText.setFont(font);
    pointsLabelsText.setString("Coins\nFruits\nGhosts");
    pointsLabelsText.setCharacterSize(18);
    pointsLabelsText.setFillColor(sf::Color(180, 180, 180));
    pointsLabelsText.setLineSpacing(1.6f);
    pointsLabelsText.setPosition(rightSidebarX, 435);

    pointsValuesText.setFont(font);
    pointsValuesText.setString("10PTS\n50PTS\n200PTS");
    pointsValuesText.setCharacterSize(18);
    pointsValuesText.setFillColor(sf::Color::White);
    pointsValuesText.setLineSpacing(1.6f);
    pointsValuesText.setPosition(rightSidebarX + 100, 435);
}

void LevelState::onExit() {
    std::cout << "Exiting LevelState" << std::endl;
}

void LevelState::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            finish(StateAction::PUSH, std::make_unique<PausedState>());
        }
    }

    if (!showingReady) {
        handlePlayerInput();
    }
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
    bool isDeathAnimPlaying = world->isPlayingDeathAnimation();

    if (wasPlayingDeathAnimation && !isDeathAnimPlaying) {
        showingReady = true;
        readyTimer = 0.0f;
    }

    wasPlayingDeathAnimation = isDeathAnimPlaying;

    if (showingReady) {
        readyTimer += deltaTime;
        if (readyTimer >= READY_DURATION) {
            showingReady = false;
        }
    }
    else {
        elapsedTime += deltaTime;
    }
    world->update(showingReady ? 0.0f : deltaTime);
    updateUI();
    checkGameState();
}

void LevelState::updateUI() {
    auto& score = world->getScore();
    auto* pacman = world->getPacMan();

    scoreText.setString(std::to_string(score.getCurrentScore()));

    if (pacman) {
        livesText.setString(std::to_string(pacman->getLives()));
    }

    levelText.setString(std::to_string(currentLevel));

    int minutes = static_cast<int>(elapsedTime) / 60;
    int seconds = static_cast<int>(elapsedTime) % 60;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes
        << ":" << std::setfill('0') << std::setw(2) << seconds;
    timerText.setString(oss.str());
}

void LevelState::checkGameState() {
    if (world->isGameOver()) {
        int finalScore = world->getScore().getCurrentScore();
        finish(StateAction::SWITCH,
               std::make_unique<VictoryState>(false, finalScore, currentLevel));
    } else if (world->isLevelComplete()) {
        int finalScore = world->getScore().getCurrentScore();

        if (tutorialMode) {
            finish(StateAction::SWITCH,
                   std::make_unique<VictoryState>(true, finalScore, 0));
        } else {
            finish(StateAction::SWITCH,
                   std::make_unique<VictoryState>(true, finalScore, currentLevel));
        }
    }
}

void LevelState::render(sf::RenderWindow& window) {
    window.clear(sf::Color(15, 15, 15));

    auto cam = camera.get();  // Direct access to shared_ptr

    if (hasLetterboxing) {
        sf::RectangleShape leftBg;
        leftBg.setSize(sf::Vector2f(cam->getViewportOffsetX(), cam->getWindowHeight()));
        leftBg.setFillColor(sf::Color(20, 20, 30));
        leftBg.setPosition(0, 0);
        window.draw(leftBg);

        sf::RectangleShape rightBg;
        rightBg.setSize(sf::Vector2f(cam->getViewportOffsetX(), cam->getWindowHeight()));
        rightBg.setFillColor(sf::Color(20, 20, 30));
        rightBg.setPosition(cam->getViewportOffsetX() + cam->getViewportWidth(), 0);
        window.draw(rightBg);

        sf::RectangleShape leftLine(sf::Vector2f(3, cam->getWindowHeight()));
        leftLine.setFillColor(sf::Color(255, 215, 0));
        leftLine.setPosition(cam->getViewportOffsetX() - 3, 0);
        window.draw(leftLine);

        sf::RectangleShape rightLine(sf::Vector2f(3, cam->getWindowHeight()));
        rightLine.setFillColor(sf::Color(255, 215, 0));
        rightLine.setPosition(cam->getViewportOffsetX() + cam->getViewportWidth(), 0);
        window.draw(rightLine);

        float dividerWidth = cam->getViewportOffsetX() - 40;
        if (dividerWidth > 0) {
            sf::RectangleShape divider1(sf::Vector2f(dividerWidth, 2));
            divider1.setFillColor(sf::Color(80, 80, 90));
            divider1.setPosition(20, 220);
            window.draw(divider1);

            sf::RectangleShape divider2(sf::Vector2f(dividerWidth, 2));
            divider2.setFillColor(sf::Color(80, 80, 90));
            divider2.setPosition(20, 340);
            window.draw(divider2);
        }

        float rightDividerX = cam->getViewportOffsetX() + cam->getViewportWidth() + 20;
        if (dividerWidth > 0) {
            sf::RectangleShape divider3(sf::Vector2f(dividerWidth, 2));
            divider3.setFillColor(sf::Color(80, 80, 90));
            divider3.setPosition(rightDividerX, 230);
            window.draw(divider3);

            sf::RectangleShape divider4(sf::Vector2f(dividerWidth, 2));
            divider4.setFillColor(sf::Color(80, 80, 90));
            divider4.setPosition(rightDividerX, 390);
            window.draw(divider4);
        }

        window.draw(scoreTitle);
        window.draw(scoreText);
        window.draw(livesTitle);
        window.draw(livesText);
        window.draw(levelTitle);
        window.draw(levelText);

        window.draw(timerTitle);
        window.draw(timerText);
        window.draw(controlsTitle);
        window.draw(controlKeysText);
        window.draw(controlActionsText);
        window.draw(pointsTitle);
        window.draw(pointsLabelsText);
        window.draw(pointsValuesText);
    }
    else {
        sf::Text compactScore;
        compactScore.setFont(font);
        compactScore.setString("SCORE: " + std::to_string(world->getScore().getCurrentScore()));
        compactScore.setCharacterSize(20);
        compactScore.setFillColor(sf::Color::Yellow);
        compactScore.setPosition(10, 10);
        window.draw(compactScore);

        sf::Text compactLives;
        compactLives.setFont(font);
        compactLives.setString("LIVES: " + std::to_string(world->getPacMan()->getLives()));
        compactLives.setCharacterSize(20);
        compactLives.setFillColor(sf::Color::White);
        compactLives.setPosition(10, 40);
        window.draw(compactLives);

        sf::Text compactLevel;
        compactLevel.setFont(font);
        compactLevel.setString("LEVEL: " + std::to_string(currentLevel));
        compactLevel.setCharacterSize(20);
        compactLevel.setFillColor(sf::Color::White);
        compactLevel.setPosition(10, 70);
        window.draw(compactLevel);
    }

    for (const auto& view : factory->getViews()) {
        view->draw(window);
    }

    if (showingReady) {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(cam->getViewportWidth(), cam->getViewportHeight()));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        overlay.setPosition(cam->getViewportOffsetX(), cam->getViewportOffsetY());
        window.draw(overlay);

        window.draw(readyText);
    }
}

} // namespace pacman::representation