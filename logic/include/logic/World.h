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

struct MapDimensions {
    int rows;
    int cols;
};

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

    MapDimensions getMapDimensions() const { return {mapRows, mapCols}; }

    // ✅ Get ghost spawn center position (for spawning ghosts to find exit)
    Position getGhostCenterPosition() const { return ghostCenterPosition; }

    // ✅ NOW PUBLIC: Used by Ghost AI to determine viable directions
    bool canMoveInDirection(const Position& pos, Direction dir, float radius) const;

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

    int mapRows = 0;
    int mapCols = 0;

    // Collision detection
    void handleCollisions();
    bool checkWallCollision(const Position& pos, Direction dir) const;

    bool isPositionBlocked(const Position& pos, float radius) const;

    bool isAtIntersection(const Position& pos, Direction currentDir, float radius) const;

    // Level loading helpers
    void parseMap(const std::string& mapFile);
    void spawnEntities(const std::vector<std::string>& mapData);
    Position gridToWorld(int row, int col, int totalRows, int totalCols) const;

    // Collision detection helpers
    void updatePacManWithCollisions(float deltaTime);
    bool checkWallCollision(const Position& pos, float radius) const;

    // ✅ UPDATED: Now handles forced direction changes when ghost hits wall
    void updateGhostWithCollisions(Ghost* ghost, float deltaTime);
    
    // Difficulty scaling
    void applyDifficultyScaling();
};

} // namespace pacman