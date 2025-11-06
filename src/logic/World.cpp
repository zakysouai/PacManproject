// src/logic/World.cpp - UPDATED met file loading
#include "World.h"
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

namespace PacMan {
namespace Logic {

World::World(int width, int height)
    : m_width(width), m_height(height),
      m_pacmanSpawn(10, 9) {   // Default spawn (wordt overschreven bij file load)

    // Initialize maze with empty tiles
    m_maze.resize(m_height);
    for (auto& row : m_maze) {
        row.resize(m_width, TileType::Empty);
    }
}

bool World::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open map file: " << filename << std::endl;
        return false;
    }

    std::vector<std::string> lines;
    std::string line;

    // Lees alle regels
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    if (lines.empty()) {
        std::cerr << "ERROR: Map file is empty!" << std::endl;
        return false;
    }

    // Update maze dimensions based on file
    m_height = static_cast<int>(lines.size());
    m_width = 0;
    for (const auto& l : lines) {
        if (static_cast<int>(l.length()) > m_width) {
            m_width = static_cast<int>(l.length());
        }
    }

    // Clear en resize maze
    m_maze.clear();
    m_maze.resize(m_height);
    m_ghostSpawns.clear();

    std::cout << "Loading map from file: " << filename << std::endl;
    std::cout << "Map dimensions: " << m_width << "x" << m_height << std::endl;

    // Parse elke regel
    for (int y = 0; y < m_height; ++y) {
        m_maze[y].resize(m_width, TileType::Empty);

        const std::string& currentLine = lines[y];

        for (int x = 0; x < static_cast<int>(currentLine.length()); ++x) {
            char c = currentLine[x];

            switch (c) {
                case 'x':
                case 'X':
                    m_maze[y][x] = TileType::Wall;
                    break;

                case 'p':
                case 'P':
                    // PacMan spawn positie
                    m_pacmanSpawn = {x, y};
                    m_maze[y][x] = TileType::Empty;  // Maak tile zelf leeg
                    std::cout << "PacMan spawn found at (" << x << ", " << y << ")" << std::endl;
                    break;

                case 'g':
                case 'h':
                case 'j':
                case 'k':
                case 'G':
                case 'H':
                case 'J':
                case 'K':
                    // Ghost spawn posities
                    m_ghostSpawns.push_back({x, y});
                    m_maze[y][x] = TileType::GhostSpawn;
                    std::cout << "Ghost spawn '" << c << "' found at (" << x << ", " << y << ")" << std::endl;
                    break;

                case 'F':
                case 'f':
                    // Fruit
                    m_maze[y][x] = TileType::Fruit;
                    break;

                case '.':
                    // Explicit coin marker
                    m_maze[y][x] = TileType::Coin;
                    break;

                case 'O':
                case 'o':
                    // Power pellet
                    m_maze[y][x] = TileType::PowerPellet;
                    break;

                case ' ':
                    // Spatie = coin (standaard gameplay)
                    m_maze[y][x] = TileType::Coin;
                    break;

                case '-':
                case '_':
                    // Expliciet lege tile (geen coin)
                    m_maze[y][x] = TileType::Empty;
                    break;

                default:
                    // Onbekend karakter = empty
                    m_maze[y][x] = TileType::Empty;
                    break;
            }
        }

        // Vul rest van de rij met empty als de regel korter is
        for (int x = static_cast<int>(currentLine.length()); x < m_width; ++x) {
            m_maze[y][x] = TileType::Empty;
        }
    }

    // Voeg power pellets toe in de hoeken (optioneel)
    // Alleen als er ruimte is en het niet al een wall is
    auto tryAddPowerPellet = [this](int x, int y) {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            if (m_maze[y][x] == TileType::Coin || m_maze[y][x] == TileType::Empty) {
                m_maze[y][x] = TileType::PowerPellet;
            }
        }
    };

    // Power pellets in hoeken
    tryAddPowerPellet(1, 1);                    // Top-left
    tryAddPowerPellet(m_width - 2, 1);          // Top-right
    tryAddPowerPellet(1, m_height - 2);         // Bottom-left
    tryAddPowerPellet(m_width - 2, m_height - 2); // Bottom-right

    std::cout << "Map loaded successfully!" << std::endl;
    std::cout << "  PacMan spawn: (" << m_pacmanSpawn.first << ", " << m_pacmanSpawn.second << ")" << std::endl;
    std::cout << "  Ghost spawns: " << m_ghostSpawns.size() << " locations" << std::endl;

    return true;
}

void World::loadStandardMaze() {
    // Fallback: probeer eerst map.txt te laden
    if (loadFromFile("../resources/map.txt")) {
        return;  // Succesvol geladen
    }

    // Als dat niet lukt, probeer andere locaties
    if (loadFromFile("resources/map.txt")) {
        return;
    }

    if (loadFromFile("map.txt")) {
        return;
    }

    std::cerr << "WARNING: Could not load map.txt, using hardcoded maze" << std::endl;

    // FALLBACK: Hardcoded maze (je oude code als backup)
    const std::vector<std::string> mazeLayout = {
        "xxxxxxxxxxxxxxxxxxxx",
        "x    x        x    x",
        "x xx x xxxxxx x xx x",
        "x x              x x",
        "x x xx xx  xx xx x x",
        "x      x    x      x",
        "x x xx xxxxxx xx x x",
        "x x              x x",
        "x xx x xxxxxx x xx x",
        "x    x    p   x    x",
        "xxxxxxxxxxxxxxxxxxxx"
    };

    m_height = static_cast<int>(mazeLayout.size());
    m_width = static_cast<int>(mazeLayout[0].length());

    m_maze.clear();
    m_maze.resize(m_height);
    m_ghostSpawns.clear();

    for (int y = 0; y < m_height; ++y) {
        m_maze[y].resize(m_width);
        for (int x = 0; x < m_width; ++x) {
            char c = mazeLayout[y][x];
            if (c == 'x') {
                m_maze[y][x] = TileType::Wall;
            } else if (c == 'p') {
                m_pacmanSpawn = {x, y};
                m_maze[y][x] = TileType::Empty;
            } else if (c == ' ') {
                m_maze[y][x] = TileType::Coin;
            } else {
                m_maze[y][x] = TileType::Empty;
            }
        }
    }

    // Default ghost spawn (centrum)
    m_ghostSpawns.push_back({m_width / 2, m_height / 2});

    // Power pellets
    m_maze[1][1] = TileType::PowerPellet;
    m_maze[1][m_width - 2] = TileType::PowerPellet;
    m_maze[m_height - 2][1] = TileType::PowerPellet;
    m_maze[m_height - 2][m_width - 2] = TileType::PowerPellet;
}

void World::update(float deltaTime) {
    // Update game entities (to be implemented)
}

TileType World::getTileAt(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return TileType::Wall;
    }
    return m_maze[y][x];
}

void World::setTileAt(int x, int y, TileType type) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_maze[y][x] = type;
    }
}

std::pair<int, int> World::normalizedToGrid(float normalizedX, float normalizedY) const {
    int gridX = static_cast<int>((normalizedX + 1.0f) * 0.5f * m_width);
    int gridY = static_cast<int>((normalizedY + 1.0f) * 0.5f * m_height);

    gridX = std::max(0, std::min(gridX, m_width - 1));
    gridY = std::max(0, std::min(gridY, m_height - 1));

    return {gridX, gridY};
}

std::pair<float, float> World::gridToNormalized(int gridX, int gridY) const {
    float normalizedX = (static_cast<float>(gridX) / m_width) * 2.0f - 1.0f;
    float normalizedY = (static_cast<float>(gridY) / m_height) * 2.0f - 1.0f;
    
    return {normalizedX, normalizedY};
}

bool World::isWalkable(float normalizedX, float normalizedY) const {
    auto [gridX, gridY] = normalizedToGrid(normalizedX, normalizedY);
    TileType tile = getTileAt(gridX, gridY);
    return tile != TileType::Wall;
}

} // namespace Logic
} // namespace PacMan