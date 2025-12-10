#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

namespace pacman::representation {

class TutorialIntroState : public State {
public:
    TutorialIntroState();
    ~TutorialIntroState() override = default;
    
    void onEnter() override;
    void onExit() override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
private:
    sf::Font font;
    sf::Text titleText;
    sf::Text instructionsText;
    sf::Text controlsText;
    sf::Text continueText;
    
    void loadFont();
    void setupTexts();
};

} // namespace pacman::representation