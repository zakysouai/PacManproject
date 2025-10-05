#ifndef WORLD_H
#define WORLD_H

#include "TileType.h"
#include <vector>
#include <memory>

namespace PacMan {
namespace Logic {

/**
 * @brief Represents the game world containing the maze and all entities
 * 
 * The World uses normalized coordinates where the maze spans [-1, 1] in both dimensions.
 * This makes the game logic independent of screen resolution.
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
     * @param normalizedX X in range [-1, 1]
     * @param normalizedY Y in range [-1, 1]
     * @return Pair of grid coordinates
     */
    std::pair<int, int> normalizedToGrid(float normalizedX, float normalizedY) const;

    /**
     * @brief Convert grid coordinates to normalized coordinates
     * @param gridX X coordinate in grid
     * @param gridY Y coordinate in grid
     * @return Pair of normalized coordinates
     */
    std::pair<float, float> gridToNormalized(int gridX, int gridY) const;

    /**
     * @brief Check if a position is walkable (not a wall)
     * @param normalizedX X in range [-1, 1]
     * @param normalizedY Y in range [-1, 1]
     * @return true if position is walkable
     */
    bool isWalkable(float normalizedX, float normalizedY) const;

    // Getters
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    const std::vector<std::vector<TileType>>& getMaze() const { return m_maze; }

private:
    int m_width;
    int m_height;
    std::vector<std::vector<TileType>> m_maze;
};

} // namespace Logic
} // namespace PacMan

#endif // WORLD_H