#include "World.h"
#include <stdexcept>
#include <cmath>

namespace PacMan {
namespace Logic {

World::World(int width, int height) 
    : m_width(width), m_height(height),
      m_pacmanSpawn(14, 23),  // Pac-Man spawnt in onderste helft (waar rode pijl wijst)
      m_ghostSpawn(14, 14) {   // Ghosts spawnen in het centrum

    // Initialize maze with empty tiles
    m_maze.resize(m_height);
    for (auto& row : m_maze) {
        row.resize(m_width, TileType::Empty);
    }
}

void World::loadStandardMaze() {
    // Standard Pac-Man maze layout (28x31 tiles is typical)
    // For simplicity, we'll create a 28x31 maze
    // W = Wall, . = Coin, P = Power Pellet, E = Empty, G = Ghost Spawn

    const std::vector<std::string> mazeLayout = {
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
        "W............WW............W",
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "W..........................W",
        "W.WWWW.WW.WWWWWWWW.WW.WWWW.W",
        "W.WWWW.WW.WWWWWWWW.WW.WWWW.W",
        "W......WW....WW....WW......W",
        "WWWWWW.WWWWW.WW.WWWWW.WWWWWW",
        "EEEEWW.WWWWW.WW.WWWWW.WWEEEE",
        "EEEEWW.WW..........WW.WWEEEE",
        "EEEEWW.WW.WWW--WWW.WW.WWEEEE",
        "WWWWWW.WW.W......W.WW.WWWWWW",
        "......... W.GGGG.W .........",
        "WWWWWW.WW.W......W.WW.WWWWWW",
        "EEEEWW.WW.WWWWWWWW.WW.WWEEEE",
        "EEEEWW.WW..........WW.WWEEEE",
        "EEEEWW.WW.WWWWWWWW.WW.WWEEEE",
        "WWWWWW.WW.WWWWWWWW.WW.WWWWWW",
        "W............WW............W",
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "W...WW.......  .......WW...W",  // Rij 23: Pac-Man spawn positie (14, 23)
        "WWW.WW.WW.WWWWWWWW.WW.WW.WWW",
        "WWW.WW.WW.WWWWWWWW.WW.WW.WWW",
        "W......WW....WW....WW......W",
        "W.WWWWWWWWWW.WW.WWWWWWWWWW.W",
        "W.WWWWWWWWWW.WW.WWWWWWWWWW.W",
        "W..........................W",
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWW"
    };

    // Resize maze to match layout
    m_height = static_cast<int>(mazeLayout.size());
    m_width = static_cast<int>(mazeLayout[0].length());

    m_maze.clear();
    m_maze.resize(m_height);

    for (int y = 0; y < m_height; ++y) {
        m_maze[y].resize(m_width);
        for (int x = 0; x < m_width; ++x) {
            char c = mazeLayout[y][x];
            switch (c) {
                case 'W':
                    m_maze[y][x] = TileType::Wall;
                    break;
                case '.':
                    m_maze[y][x] = TileType::Coin;
                    break;
                case 'P':
                    m_maze[y][x] = TileType::PowerPellet;
                    break;
                case 'G':
                    m_maze[y][x] = TileType::GhostSpawn;
                    break;
                case 'F':
                    m_maze[y][x] = TileType::Fruit;
                    break;
                case '-':
                case ' ':
                case 'E':
                default:
                    m_maze[y][x] = TileType::Empty;
                    break;
            }
        }
    }

    // Add power pellets at corners
    if (m_width > 0 && m_height > 0) {
        m_maze[1][1] = TileType::PowerPellet;                    // Top-left
        m_maze[1][m_width - 2] = TileType::PowerPellet;          // Top-right
        m_maze[m_height - 2][1] = TileType::PowerPellet;         // Bottom-left
        m_maze[m_height - 2][m_width - 2] = TileType::PowerPellet; // Bottom-right
    }

    // Update spawn positions based on actual maze
    m_pacmanSpawn = {14, 23};  // Onderste helft, waar rode pijl wijst
    m_ghostSpawn = {14, 14};   // Centrum van de maze (bij de G's)
}

void World::update(float deltaTime) {
    // Update game entities (to be implemented)
}

TileType World::getTileAt(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return TileType::Wall; // Out of bounds treated as wall
    }
    return m_maze[y][x];
}

void World::setTileAt(int x, int y, TileType type) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_maze[y][x] = type;
    }
}

std::pair<int, int> World::normalizedToGrid(float normalizedX, float normalizedY) const {
    // Convert from [-1, 1] to [0, width/height]
    int gridX = static_cast<int>((normalizedX + 1.0f) * 0.5f * m_width);
    int gridY = static_cast<int>((normalizedY + 1.0f) * 0.5f * m_height);
    
    // Clamp to valid range
    gridX = std::max(0, std::min(gridX, m_width - 1));
    gridY = std::max(0, std::min(gridY, m_height - 1));
    
    return {gridX, gridY};
}

std::pair<float, float> World::gridToNormalized(int gridX, int gridY) const {
    // Convert from [0, width/height] to [-1, 1]
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