#include "representation/states/LevelState.h"
#include "representation/states/PausedState.h"
#include "representation/states/VictoryState.h"
#include "logic/utils/Stopwatch.h"
#include <iostream>

namespace pacman::representation {

LevelState::LevelState(int level)
    : currentLevel(level) {
}

void LevelState::onEnter() {
    std::cout << "Entering LevelState (Level " << currentLevel << ")" << std::endl;
    
    // Create camera
    camera = std::make_unique<pacman::Camera>(800, 600);
    
    // Create factory
    factory = std::make_unique<ConcreteFactory>(*camera);
    
    // Create world
    world = std::make_unique<pacman::World>(factory.get());
    
    // Load level - now loads FULL map with all entities
    world->loadLevel("resources/maps/map.txt");

    // Setup UI
    loadFont();
    setupUI();
}

void LevelState::onExit() {
    std::cout << "Exiting LevelState" << std::endl;
}

void LevelState::loadFont() {
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "Warning: Could not load font" << std::endl;
    }
}

void LevelState::setupUI() {
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(10, 10);

    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(10, 40);

    levelText.setFont(font);
    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(10, 70);
}

void LevelState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            // Pause game
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
    // Update world
    world->update(deltaTime);

    for (const auto& view : factory->getViews()) {
        view->update(deltaTime);
    }

    // Update UI
    updateUI();

    // Check game state
    checkGameState();
}
void LevelState::updateUI() {
    auto* score = world->getScore();
    auto* pacman = world->getPacMan();

    if (score) {
        scoreText.setString("SCORE: " + std::to_string(score->getCurrentScore()));
    }

    if (pacman) {
        livesText.setString("LIVES: " + std::to_string(pacman->getLives()));
    }

    levelText.setString("LEVEL: " + std::to_string(currentLevel));
}

void LevelState::checkGameState() {
    if (world->isGameOver()) {
        // Game over
        int finalScore = world->getScore()->getCurrentScore();
        finish(StateAction::SWITCH, std::make_unique<VictoryState>(false, finalScore));
    } else if (world->isLevelComplete()) {
        // Level complete
        int finalScore = world->getScore()->getCurrentScore();
        finish(StateAction::SWITCH, std::make_unique<VictoryState>(true, finalScore));
    }
}

void LevelState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    // Draw all entities through their views
    for (const auto& view : factory->getViews()) {
        view->draw(window);
    }
    
    // Draw UI
    window.draw(scoreText);
    window.draw(livesText);
    window.draw(levelText);
}

} // namespace pacman::representation