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

    // Level loading helpers
    void parseMap(const std::string& mapFile);
    void spawnEntities(const std::vector<std::string>& mapData);
    Position gridToWorld(int row, int col, int totalRows, int totalCols) const;
    
    // Difficulty scaling
    void applyDifficultyScaling();
};

} // namespace pacman