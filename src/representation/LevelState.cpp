// src/representation/LevelState.cpp (UPDATE)
#include "LevelState.h"
#include "TileType.h"
#include "SpriteManager.h"
#include <SFML/Graphics.hpp>

namespace PacMan {

LevelState::LevelState() {
    // Create world with standard maze
    m_world = std::make_unique<Logic::World>(28, 31);
    m_world->loadStandardMaze();

    // Create entity views
    m_pacmanView = std::make_unique<PacManView>();

    // Create 4 ghost views (één van elke kleur)
    m_ghostViews.push_back(std::make_unique<GhostView>(GhostColor::Red));
    m_ghostViews.push_back(std::make_unique<GhostView>(GhostColor::Pink));
    m_ghostViews.push_back(std::make_unique<GhostView>(GhostColor::Blue));
    m_ghostViews.push_back(std::make_unique<GhostView>(GhostColor::Orange));
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
        // Bijvoorbeeld:
        // if (event.key.code == sf::Keyboard::Right) {
        //     m_pacmanView->setDirection(Logic::Direction::Right);
        // }
    }
}

void LevelState::update(float deltaTime, StateManager& manager) {
    // Update world logic
    m_world->update(deltaTime);

    // Update entity views
    m_pacmanView->update(deltaTime);
    for (auto& ghostView : m_ghostViews) {
        ghostView->update(deltaTime);
    }
}

void LevelState::render(sf::RenderWindow& window) {
    // Initialize camera if not done yet
    if (!m_camera) {
        m_camera = std::make_unique<Camera>(window.getSize().x, window.getSize().y);
    }

    window.clear(sf::Color::Black);

    // Render maze
    renderMaze(window);

    // Render entities
    renderEntities(window);

    window.display();
}

void LevelState::renderMaze(sf::RenderWindow& window) {
    const auto& maze = m_world->getMaze();
    int width = m_world->getWidth();
    int height = m_world->getHeight();

    // Calculate tile size
    sf::Vector2f tileSize = m_camera->getTileSize(width, height);

    // Get sprite manager voor coins en fruits
    auto spriteManager = SpriteManager::getInstance();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Logic::TileType tile = m_world->getTileAt(x, y);

            // Convert grid to normalized then to screen
            auto [normX, normY] = m_world->gridToNormalized(x, y);
            sf::Vector2f screenPos = m_camera->normalizedToScreen(normX, normY);

            switch (tile) {
                case Logic::TileType::Wall: {
                    sf::RectangleShape shape;
                    shape.setPosition(screenPos);
                    shape.setSize(tileSize);
                    shape.setFillColor(sf::Color::Blue);
                    window.draw(shape);
                    break;
                }

                case Logic::TileType::Coin: {
                    // Draw small circle for coin (of gebruik sprite)
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
                    // Gebruik fruit sprite
                    sf::Sprite fruitSprite;
                    fruitSprite.setTexture(spriteManager->getTexture());
                    fruitSprite.setTextureRect(spriteManager->getSpriteRect(SpriteType::Cherry));

                    // Schaal de sprite
                    float scale = tileSize.x / 16.0f;  // 16 is de sprite grootte
                    fruitSprite.setScale(scale, scale);

                    // Centreer
                    sf::IntRect rect = fruitSprite.getTextureRect();
                    fruitSprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
                    fruitSprite.setPosition(screenPos.x + tileSize.x * 0.5f,
                                           screenPos.y + tileSize.y * 0.5f);

                    window.draw(fruitSprite);
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

void LevelState::renderEntities(sf::RenderWindow& window) {
    // TODO: Krijg de actuele posities van entities uit de World
    // Voor nu: dummy posities om sprites te testen

    int width = m_world->getWidth();
    int height = m_world->getHeight();
    sf::Vector2f tileSize = m_camera->getTileSize(width, height);

    // Schaal voor sprites (1 tile = 1 sprite)
    float spriteScale = tileSize.x / 16.0f;  // 16 is de base sprite size

    // Render PacMan (bijvoorbeeld in het midden)
    auto [pacmanNormX, pacmanNormY] = m_world->gridToNormalized(14, 15);
    sf::Vector2f pacmanPos = m_camera->normalizedToScreen(pacmanNormX, pacmanNormY);
    pacmanPos.x += tileSize.x * 0.5f;
    pacmanPos.y += tileSize.y * 0.5f;
    m_pacmanView->draw(window, pacmanPos, sf::Vector2f(spriteScale, spriteScale));

    // Render Ghosts (bijvoorbeeld rondom het centrum)
    std::vector<std::pair<int, int>> ghostPositions = {
        {13, 14}, {14, 14}, {13, 15}, {14, 15}
    };

    for (size_t i = 0; i < m_ghostViews.size() && i < ghostPositions.size(); ++i) {
        auto [gx, gy] = ghostPositions[i];
        auto [ghostNormX, ghostNormY] = m_world->gridToNormalized(gx, gy);
        sf::Vector2f ghostPos = m_camera->normalizedToScreen(ghostNormX, ghostNormY);
        ghostPos.x += tileSize.x * 0.5f;
        ghostPos.y += tileSize.y * 0.5f;
        m_ghostViews[i]->draw(window, ghostPos, sf::Vector2f(spriteScale, spriteScale));
    }
}

} // namespace PacMan