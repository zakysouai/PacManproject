// src/representation/LevelState.cpp - UPDATED voor file loading
#include "LevelState.h"

#include <iostream>

#include "TileType.h"
#include "SpriteManager.h"
#include <SFML/Graphics.hpp>

namespace PacMan {

LevelState::LevelState() {
    // Create world - dimensies worden automatisch bepaald door map.txt
    m_world = std::make_unique<Logic::World>(20, 11);  // Default size
    m_world->loadStandardMaze();  // Dit laadt nu map.txt!

    // Create entity views
    m_pacmanView = std::make_unique<PacManView>();

    // Create 4 ghost views (voor de 4 spawns: g, h, j, k)
    m_ghostViews.push_back(std::make_unique<GhostView>(GhostColor::Red));
    m_ghostViews.push_back(std::make_unique<GhostView>(GhostColor::Pink));
    m_ghostViews.push_back(std::make_unique<GhostView>(GhostColor::Blue));
    m_ghostViews.push_back(std::make_unique<GhostView>(GhostColor::Orange));
}

void LevelState::onEnter() {
    // Reset if needed
}

void LevelState::onExit() {
    // Cleanup
}

void LevelState::handleInput(const sf::Event& event, StateManager& manager) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            // TODO: Push PausedState
        }

        // TODO: Handle arrow keys for movement
    }
}

void LevelState::update(float deltaTime, StateManager& manager) {
    m_world->update(deltaTime);
    m_pacmanView->update(deltaTime);

    for (auto& ghostView : m_ghostViews) {
        ghostView->update(deltaTime);
    }
}

void LevelState::render(sf::RenderWindow& window) {
    if (!m_camera) {
        m_camera = std::make_unique<Camera>(window.getSize().x, window.getSize().y);
    }

    window.clear(sf::Color::Black);

    renderMaze(window);
    renderEntities(window);

    window.display();
}

void LevelState::renderMaze(sf::RenderWindow& window) {
    const auto& maze = m_world->getMaze();
    int width = m_world->getWidth();
    int height = m_world->getHeight();

    // Calculate tile size
    sf::Vector2f tileSize = m_camera->getTileSize(width, height);

    auto spriteManager = SpriteManager::getInstance();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Logic::TileType tile = m_world->getTileAt(x, y);

            auto [normX, normY] = m_world->gridToNormalized(x, y);
            sf::Vector2f screenPos = m_camera->normalizedToScreen(normX, normY);

            switch (tile) {
                case Logic::TileType::Wall: {
                    sf::RectangleShape shape;
                    shape.setPosition(screenPos);
                    shape.setSize(tileSize);
                    shape.setFillColor(sf::Color(33, 33, 222)); // Blauw
                    shape.setOutlineColor(sf::Color(66, 66, 255));
                    shape.setOutlineThickness(1.0f);
                    window.draw(shape);
                    break;
                }

                case Logic::TileType::Coin: {
                    // Kleine coin
                    sf::CircleShape coin(tileSize.x * 0.12f);
                    coin.setPosition(screenPos.x + tileSize.x * 0.38f,
                                    screenPos.y + tileSize.y * 0.38f);
                    coin.setFillColor(sf::Color(255, 184, 174)); // Beige/roze
                    window.draw(coin);
                    break;
                }

                case Logic::TileType::PowerPellet: {
                    // Grotere power pellet
                    sf::CircleShape pellet(tileSize.x * 0.25f);
                    pellet.setPosition(screenPos.x + tileSize.x * 0.25f,
                                      screenPos.y + tileSize.y * 0.25f);
                    pellet.setFillColor(sf::Color::White);
                    window.draw(pellet);
                    break;
                }

                case Logic::TileType::Fruit: {
                    sf::Sprite fruitSprite;
                    fruitSprite.setTexture(spriteManager->getTexture());
                    fruitSprite.setTextureRect(spriteManager->getSpriteRect(SpriteType::Cherry));

                    float scale = tileSize.x / 13.0f;  // 13 = fruit sprite size
                    fruitSprite.setScale(scale, scale);

                    sf::IntRect rect = fruitSprite.getTextureRect();
                    fruitSprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
                    fruitSprite.setPosition(screenPos.x + tileSize.x * 0.5f,
                                           screenPos.y + tileSize.y * 0.5f);

                    window.draw(fruitSprite);
                    break;
                }

                case Logic::TileType::GhostSpawn: {
                    // Optioneel: teken een marker voor ghost spawn locatie
                    // (meestal leeg, maar kan handig zijn voor debugging)
                    sf::CircleShape marker(tileSize.x * 0.1f);
                    marker.setPosition(screenPos.x + tileSize.x * 0.4f,
                                      screenPos.y + tileSize.y * 0.4f);
                    marker.setFillColor(sf::Color(100, 100, 100, 100)); // Grijs, semi-transparent
                    // window.draw(marker);  // Uncomment voor debug
                    break;
                }

                case Logic::TileType::Empty:
                default:
                    // Niets tekenen
                    break;
            }
        }
    }
}

void LevelState::renderEntities(sf::RenderWindow& window) {
    int width = m_world->getWidth();
    int height = m_world->getHeight();
    sf::Vector2f tileSize = m_camera->getTileSize(width, height);

    // DEBUG: Print tile size
    static bool firstTime = true;
    if (firstTime) {
        std::cout << "=== RENDER DEBUG ===" << std::endl;
        std::cout << "Tile size: " << tileSize.x << "x" << tileSize.y << std::endl;
        firstTime = false;
    }

    // AANGEPASTE SCALE BEREKENING
    // PacMan sprites: 13×13 pixels
    // Ghost sprites: 16×16 pixels (NIET 14×14!)
    float pacmanScale = (tileSize.x * 0.8f) / 13.0f;
    float ghostScale = (tileSize.x * 0.8f) / 16.0f;  // <-- AANGEPAST: 16 ipv 14!

    std::cout << "PacMan scale: " << pacmanScale << std::endl;
    std::cout << "Ghost scale: " << ghostScale << std::endl;

    // Render PacMan
    auto [pacmanGridX, pacmanGridY] = m_world->getPacManSpawnPosition();
    auto [pacmanNormX, pacmanNormY] = m_world->gridToNormalized(pacmanGridX, pacmanGridY);
    sf::Vector2f pacmanPos = m_camera->normalizedToScreen(pacmanNormX, pacmanNormY);
    pacmanPos.x += tileSize.x * 0.5f;
    pacmanPos.y += tileSize.y * 0.5f;
    m_pacmanView->draw(window, pacmanPos, sf::Vector2f(pacmanScale, pacmanScale));

    // Render Ghosts
    const auto& ghostSpawns = m_world->getGhostSpawnPositions();

    for (size_t i = 0; i < m_ghostViews.size() && i < ghostSpawns.size(); ++i) {
        int gx = ghostSpawns[i].first;
        int gy = ghostSpawns[i].second;

        auto [ghostNormX, ghostNormY] = m_world->gridToNormalized(gx, gy);
        sf::Vector2f ghostPos = m_camera->normalizedToScreen(ghostNormX, ghostNormY);
        ghostPos.x += tileSize.x * 0.5f;
        ghostPos.y += tileSize.y * 0.5f;
        m_ghostViews[i]->draw(window, ghostPos, sf::Vector2f(ghostScale, ghostScale));
    }
}






} // namespace PacMan