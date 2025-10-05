#include "MenuState.h"
#include "LevelState.h"
#include "StateManager.h"
#include <fstream>
#include <algorithm>

namespace PacMan {

MenuState::MenuState() {
    // Try to load a default font - you may need to adjust the path
    if (!m_font.loadFromFile("../assets/fonts/arial.ttf")) {
        // Fallback: use system default (this might not work on all systems)
        // In production, ensure the font file exists
    }
    
    loadHighScores();
    setupUI();
}

void MenuState::onEnter() {
    // Refresh high scores when entering menu
    loadHighScores();
    setupUI();
}

void MenuState::onExit() {
    // Cleanup if needed
}

void MenuState::loadHighScores() {
    m_highScores.clear();
    
    std::ifstream file("highscores.txt");
    if (file.is_open()) {
        std::string name;
        int score;
        while (file >> name >> score) {
            m_highScores.emplace_back(name, score);
        }
        file.close();
    }
    
    // Sort by score (descending)
    std::sort(m_highScores.begin(), m_highScores.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Keep only top 5
    if (m_highScores.size() > 5) {
        m_highScores.resize(5);
    }
    
    // Add default scores if empty
    if (m_highScores.empty()) {
        m_highScores = {
            {"PLAYER", 1000},
            {"PLAYER", 750},
            {"PLAYER", 500},
            {"PLAYER", 250},
            {"PLAYER", 100}
        };
    }
}

void MenuState::setupUI() {
    // Title
    m_titleText.setFont(m_font);
    m_titleText.setString("PAC-MAN");
    m_titleText.setCharacterSize(60);
    m_titleText.setFillColor(sf::Color::Yellow);
    m_titleText.setPosition(300, 50);
    
    // Scoreboard
    m_scoreTexts.clear();
    float yPos = 150.0f;
    for (size_t i = 0; i < m_highScores.size(); ++i) {
        sf::Text scoreText;
        scoreText.setFont(m_font);
        scoreText.setString(std::to_string(i + 1) + ". " + 
                           m_highScores[i].first + " - " + 
                           std::to_string(m_highScores[i].second));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(300, yPos);
        m_scoreTexts.push_back(scoreText);
        yPos += 40.0f;
    }
    
    // Play button
    m_playButton.setSize(sf::Vector2f(200, 60));
    m_playButton.setPosition(300, 450);
    m_playButton.setFillColor(sf::Color(0, 100, 200));
    
    m_playButtonText.setFont(m_font);
    m_playButtonText.setString("PLAY");
    m_playButtonText.setCharacterSize(30);
    m_playButtonText.setFillColor(sf::Color::White);
    m_playButtonText.setPosition(360, 465);
}

bool MenuState::isMouseOverPlayButton(const sf::Vector2i& mousePos, const sf::RenderWindow& window) const {
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    return m_playButton.getGlobalBounds().contains(worldPos);
}

void MenuState::handleInput(const sf::Event& event, StateManager& manager) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Check if click is on play button
            sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            sf::FloatRect buttonBounds = m_playButton.getGlobalBounds();
            
            if (buttonBounds.contains(static_cast<float>(mousePos.x), 
                                     static_cast<float>(mousePos.y))) {
                // Transition to LevelState
                manager.pushState(std::make_unique<LevelState>());
            }
        }
    }
    
    // Handle hover effect
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
        sf::FloatRect buttonBounds = m_playButton.getGlobalBounds();
        
        m_playButtonHovered = buttonBounds.contains(
            static_cast<float>(mousePos.x), 
            static_cast<float>(mousePos.y));
        
        // Update button color based on hover state
        if (m_playButtonHovered) {
            m_playButton.setFillColor(sf::Color(0, 150, 255));
        } else {
            m_playButton.setFillColor(sf::Color(0, 100, 200));
        }
    }
}

void MenuState::update(float deltaTime, StateManager& manager) {
    // Menu is mostly static, no continuous updates needed
    // Hover state is handled in handleInput
}

void MenuState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    // Draw title
    window.draw(m_titleText);
    
    // Draw scoreboard
    for (const auto& scoreText : m_scoreTexts) {
        window.draw(scoreText);
    }
    
    // Draw play button
    window.draw(m_playButton);
    window.draw(m_playButtonText);
    
    window.display();
}

} // namespace PacMan