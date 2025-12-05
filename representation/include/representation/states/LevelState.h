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
    LevelState(int level = 1);
    ~LevelState() override = default;
    
    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
private:
    std::unique_ptr<ConcreteFactory> factory;
    std::unique_ptr<pacman::World> world;
    std::unique_ptr<pacman::Camera> camera;
    
    int currentLevel;
    float elapsedTime = 0.0f;

    sf::Font font;

    // Left sidebar
    sf::Text scoreText;
    sf::Text livesText;
    sf::Text levelText;

    // Right sidebar
    sf::Text timerText;
    sf::Text controlsTitle;
    sf::Text controlsText;
    
    void loadFont();
    void setupUI();
    void updateUI();
    void handlePlayerInput();
    void checkGameState();
};

} // namespace pacman::representation