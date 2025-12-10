// representation/include/representation/states/MenuState.h
#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "representation/states/TutorialIntroState.h"

namespace pacman::representation {

class MenuState : public State {
public:
    MenuState();
    ~MenuState() override = default;

    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text playButtonText;
    sf::Text highScoresTitle;
    std::vector<sf::Text> highScoreTexts;

    sf::RectangleShape tutorialButton;
    sf::Text tutorialButtonText;
    bool tutorialButtonHovered = false;

    sf::RectangleShape playButton;
    bool playButtonHovered = false;

    void loadFont();
    void setupTexts();  // ✅ Terug zonder parameters
    void loadHighScores();  // ✅ Terug zonder parameters
    bool isMouseOverButton(const sf::Vector2f& mousePos) const;

    sf::Vector2f mapPixelToCoords(const sf::RenderWindow& window, sf::Vector2i pixel);  // ✅ NIEUW
};

} // namespace pacman::representation