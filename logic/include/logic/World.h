#pragma once
#include "logic/EntityModel.h"
#include "logic/entities/PacMan.h"
#include "logic/entities/Ghost.h"
#include "logic/entities/Coin.h"
#include "logic/entities/Fruit.h"
#include "logic/entities/Wall.h"
#include "logic/AbstractFactory.h"
#include "logic/Score.h"
#include <vector>
#include <memory>
#include <string>

namespace pacman {

class World {
public:
    explicit World(AbstractFactory* factory);
    ~World() = default;
    
    // Game loop
    void update(float deltaTime);
    
    // Level management
    void loadLevel(const std::string& mapFile);
    void nextLevel();
    void reset();
    
    // Entity access
    PacMan* getPacMan() const { return pacman.get(); }
    const std::vector<std::unique_ptr<Ghost>>& getGhosts() const { return ghosts; }
    const std::vector<std::unique_ptr<Coin>>& getCoins() const { return coins; }
    const std::vector<std::unique_ptr<Fruit>>& getFruits() const { return fruits; }
    const std::vector<std::unique_ptr<Wall>>& getWalls() const { return walls; }
    
    // Game state
    int getCurrentLevel() const { return currentLevel; }
    bool isLevelComplete() const;
    bool isGameOver() const;
    
    // Score
    Score* getScore() { return &score; }
    
private:
    AbstractFactory* factory;  // Non-owning pointer
    
    // Entities
    std::unique_ptr<PacMan> pacman;
    std::vector<std::unique_ptr<Ghost>> ghosts;
    std::vector<std::unique_ptr<Coin>> coins;
    std::vector<std::unique_ptr<Fruit>> fruits;
    std::vector<std::unique_ptr<Wall>> walls;
    
    // Game state
    int currentLevel = 1;
    Score score;

    // Spawn positions (saved from map loading)
    Position pacmanSpawnPosition = Position(0, 0);
    Position ghostCenterPosition = Position(0, 0);

    // Death tracking to prevent multiple deaths
    float timeSinceLastDeath = 999.0f;  // Large number = no recent death
    const float DEATH_COOLDOWN = 2.0f;   // 2 seconds invulnerability

    // Collision detection
    void handleCollisions();
    bool checkWallCollision(const Position& pos, Direction dir) const;

    /**
     * @brief Check if a position is blocked by walls
     *
     * This method checks if placing an entity with the given radius at the given
     * position would cause a collision with any wall in the world.
     *
     * @param pos The position to check
     * @param radius The collision radius of the entity
     * @return true if the position is blocked (collision with wall), false otherwise
     */
    bool isPositionBlocked(const Position& pos, float radius) const;

    /**
     * @brief Check if PacMan can move in a specific direction from a position
     *
     * This tests if moving a small distance in the given direction would cause
     * a wall collision. Used to determine if a direction change is viable.
     *
     * @param pos The position to test from
     * @param dir The direction to test
     * @param radius The collision radius
     * @return true if movement is possible, false if blocked by wall
     */
    bool canMoveInDirection(const Position& pos, Direction dir, float radius) const;

    /**
     * @brief Check if PacMan is at an intersection
     *
     * An intersection is a position where PacMan can turn perpendicular to
     * the current direction. This is used to determine when direction changes
     * should be allowed.
     *
     * @param pos Current position
     * @param currentDir Current direction of movement
     * @param radius Collision radius
     * @return true if at an intersection, false otherwise
     */
    bool isAtIntersection(const Position& pos, Direction currentDir, float radius) const;

    // Level loading helpers
    void parseMap(const std::string& mapFile);
    void spawnEntities(const std::vector<std::string>& mapData);
    Position gridToWorld(int row, int col, int totalRows, int totalCols) const;

    // Collision detection helpers
    void updatePacManWithCollisions(float deltaTime);
    bool checkWallCollision(const Position& pos, float radius) const;
    
    // Difficulty scaling
    void applyDifficultyScaling();
};

} // namespace pacman