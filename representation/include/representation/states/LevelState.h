#pragma once
#include "State.h"
#include "logic/World.h"
#include "logic/Camera.h"
#include "representation/ConcreteFactory.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace pacman::representation {

class LevelState : public State {
public:
    LevelState(int level = 1, bool isTutorial = false);
    ~LevelState() override = default;
    
    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
private:
    std::shared_ptr<ConcreteFactory> factory;  // ✅ CHANGED to shared_ptr
    std::unique_ptr<pacman::World> world;
    std::shared_ptr<pacman::Camera> camera;  // ✅ CHANGED to shared_ptr
    
    int currentLevel;
    float elapsedTime = 0.0f;
    bool hasLetterboxing = false;

    sf::Font font;

    // Left sidebar
    sf::Text scoreTitle;
    sf::Text scoreText;
    sf::Text livesTitle;
    sf::Text livesText;
    sf::Text levelTitle;
    sf::Text levelText;

    // Right sidebar
    sf::Text timerTitle;
    sf::Text timerText;
    sf::Text controlsTitle;
    sf::Text controlKeysText;
    sf::Text controlActionsText;
    sf::Text pointsTitle;
    sf::Text pointsLabelsText;
    sf::Text pointsValuesText;

    bool showingReady = true;
    float readyTimer = 0.0f;
    const float READY_DURATION = 2.0f;
    sf::Text readyText;

    bool wasPlayingDeathAnimation = false;

    bool tutorialMode;
    std::string getTutorialMapPath() const;
    
    void loadFont();
    void setupUI();
    void updateUI();
    void handlePlayerInput();
    void checkGameState();
};

} // namespace pacman::representation