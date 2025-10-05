#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace PacMan {

    /**
     * @brief Main menu state showing scoreboard and play button
     */
    class MenuState : public State {
    public:
        MenuState();
        ~MenuState() override = default;

        void onEnter() override;
        void onExit() override;
        void handleInput(const sf::Event& event, StateManager& manager) override;
        void update(float deltaTime, StateManager& manager) override;
        void render(sf::RenderWindow& window) override;

    private:
        void loadHighScores();
        void setupUI();
        bool isMouseOverPlayButton(const sf::Vector2i& mousePos, const sf::RenderWindow& window) const;

        sf::Font m_font;
        sf::Text m_titleText;
        sf::Text m_playButtonText;
        sf::RectangleShape m_playButton;

        std::vector<std::pair<std::string, int>> m_highScores;
        std::vector<sf::Text> m_scoreTexts;

        bool m_playButtonHovered{false};
    };

} // namespace PacMan

#endif // MENUSTATE_H