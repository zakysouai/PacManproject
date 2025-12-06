#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace pacman::representation {

class MenuState : public State {
public:
    MenuState();
    ~MenuState() override = default;

    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text playButtonText;
    sf::Text highScoresTitle;
    std::vector<sf::Text> highScoreTexts;

    sf::RectangleShape playButton;
    bool playButtonHovered = false;

    void loadFont();
    void setupTexts();
    void loadHighScores();
    bool isMouseOverButton(const sf::Vector2f& mousePos) const;
};

} // namespace pacman::representation