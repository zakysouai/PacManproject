#ifndef TILETYPE_H
#define TILETYPE_H

namespace PacMan {
    namespace Logic {

        /**
         * @brief Enum representing different types of tiles in the maze
         */
        enum class TileType {
            Empty,          // Empty walkable space
            Wall,           // Wall (blocking)
            Coin,           // Small coin (dot)
            PowerPellet,    // Power pellet (makes ghosts vulnerable)
            Fruit,          // Bonus fruit
            GhostSpawn      // Ghost spawn point (center of maze)
        };

    } // namespace Logic
} // namespace PacMan

#endif // TILETYPE_H