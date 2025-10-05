#include "LevelState.h"
#include "TileType.h"
#include <SFML/Graphics.hpp>

namespace PacMan {

LevelState::LevelState() {
    // Create world with standard maze
    m_world = std::make_unique<Logic::World>(28, 31);
    m_world->loadStandardMaze();
    
    // Camera will be initialized in onEnter when we have window dimensions
}

void LevelState::onEnter() {
    // Reset world if needed
}

void LevelState::onExit() {
    // Cleanup
}

void LevelState::handleInput(const sf::Event& event, StateManager& manager) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            // TODO: Push PausedState
        }
        
        // TODO: Handle arrow keys for Pac-Man movement
    }
}

void LevelState::update(float deltaTime, StateManager& manager) {
    // Update world logic
    m_world->update(deltaTime);
}

void LevelState::render(sf::RenderWindow& window) {
    // Initialize camera if not done yet
    if (!m_camera) {
        m_camera = std::make_unique<Camera>(window.getSize().x, window.getSize().y);
    }
    
    window.clear(sf::Color::Black);
    
    // Render maze
    renderMaze(window);
    
    window.display();
}

void LevelState::renderMaze(sf::RenderWindow& window) {
    const auto& maze = m_world->getMaze();
    int width = m_world->getWidth();
    int height = m_world->getHeight();
    
    // Calculate tile size
    sf::Vector2f tileSize = m_camera->getTileSize(width, height);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Logic::TileType tile = m_world->getTileAt(x, y);
            
            // Convert grid to normalized then to screen
            auto [normX, normY] = m_world->gridToNormalized(x, y);
            sf::Vector2f screenPos = m_camera->normalizedToScreen(normX, normY);
            
            sf::RectangleShape shape;
            shape.setPosition(screenPos);
            
            switch (tile) {
                case Logic::TileType::Wall:
                    shape.setSize(tileSize);
                    shape.setFillColor(sf::Color::Blue);
                    window.draw(shape);
                    break;
                    
                case Logic::TileType::Coin: {
                    // Draw small circle for coin
                    sf::CircleShape coin(tileSize.x * 0.15f);
                    coin.setPosition(screenPos.x + tileSize.x * 0.35f, 
                                    screenPos.y + tileSize.y * 0.35f);
                    coin.setFillColor(sf::Color::White);
                    window.draw(coin);
                    break;
                }
                    
                case Logic::TileType::PowerPellet: {
                    // Draw larger circle for power pellet
                    sf::CircleShape pellet(tileSize.x * 0.3f);
                    pellet.setPosition(screenPos.x + tileSize.x * 0.2f, 
                                      screenPos.y + tileSize.y * 0.2f);
                    pellet.setFillColor(sf::Color::White);
                    window.draw(pellet);
                    break;
                }
                    
                case Logic::TileType::Fruit: {
                    // Draw fruit placeholder
                    sf::CircleShape fruit(tileSize.x * 0.4f);
                    fruit.setPosition(screenPos.x + tileSize.x * 0.1f, 
                                     screenPos.y + tileSize.y * 0.1f);
                    fruit.setFillColor(sf::Color::Red);
                    window.draw(fruit);
                    break;
                }
                    
                case Logic::TileType::Empty:
                case Logic::TileType::GhostSpawn:
                default:
                    // Don't draw anything for empty tiles
                    break;
            }
        }
    }
}

} // namespace PacMan