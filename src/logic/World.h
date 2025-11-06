// src/logic/World.h - UPDATED met file loading
#ifndef WORLD_H
#define WORLD_H

#include "TileType.h"
#include <vector>
#include <memory>
#include <string>

namespace PacMan {
namespace Logic {

/**
 * @brief Represents the game world containing the maze and all entities
 */
class World {
public:
    /**
     * @brief Construct a World with specified dimensions
     * @param width Width of the maze in tiles
     * @param height Height of the maze in tiles
     */
    World(int width, int height);
    ~World() = default;

    /**
     * @brief Load the standard Pac-Man maze layout
     */
    void loadStandardMaze();

    /**
     * @brief Load maze from a text file
     * @param filename Path to the map file (e.g., "../resources/map.txt")
     * @return true if loading succeeded, false otherwise
     */
    bool loadFromFile(const std::string& filename);

    /**
     * @brief Update game logic
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime);

    /**
     * @brief Get the type of tile at a specific position
     * @param x X coordinate in grid
     * @param y Y coordinate in grid
     * @return TileType at that position
     */
    TileType getTileAt(int x, int y) const;

    /**
     * @brief Set the type of tile at a specific position
     * @param x X coordinate in grid
     * @param y Y coordinate in grid
     * @param type New tile type
     */
    void setTileAt(int x, int y, TileType type);

    /**
     * @brief Convert normalized coordinates to grid coordinates
     */
    std::pair<int, int> normalizedToGrid(float normalizedX, float normalizedY) const;

    /**
     * @brief Convert grid coordinates to normalized coordinates
     */
    std::pair<float, float> gridToNormalized(int gridX, int gridY) const;

    /**
     * @brief Check if a position is walkable (not a wall)
     */
    bool isWalkable(float normalizedX, float normalizedY) const;

    // Getters
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    const std::vector<std::vector<TileType>>& getMaze() const { return m_maze; }

    /**
     * @brief Get Pac-Man's spawn position in grid coordinates
     */
    std::pair<int, int> getPacManSpawnPosition() const { return m_pacmanSpawn; }

    /**
     * @brief Get Ghost spawn positions in grid coordinates
     * @return Vector of ghost spawn positions (can be multiple)
     */
    const std::vector<std::pair<int, int>>& getGhostSpawnPositions() const {
        return m_ghostSpawns;
    }

private:
    int m_width;
    int m_height;
    std::vector<std::vector<TileType>> m_maze;
    std::pair<int, int> m_pacmanSpawn;
    std::vector<std::pair<int, int>> m_ghostSpawns;  // Meerdere ghost spawns mogelijk
};

} // namespace Logic
} // namespace PacMan

#endif // WORLD_H