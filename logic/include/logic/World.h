#pragma once
#include "logic/EntityModel.h"
#include "logic/entities/PacMan.h"
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
    
    void update(float deltaTime);

    void loadLevel(const std::string& mapFile);
    void nextLevel();
    void reset();

    PacMan* getPacMan() const { return pacman.get(); }
    const std::vector<std::unique_ptr<Coin>>& getCoins() const { return coins; }
    const std::vector<std::unique_ptr<Fruit>>& getFruits() const { return fruits; }
    const std::vector<std::unique_ptr<Wall>>& getWalls() const { return walls; }
    const std::vector<std::unique_ptr<Ghost>>& getGhosts() const { return ghosts; }

    int getCurrentLevel() const { return currentLevel; }
    bool isLevelComplete() const;
    bool isGameOver() const;

    Score* getScore() { return &score; }

    MapDimensions getMapDimensions() const { return {mapRows, mapCols}; }

    bool canMoveInDirection(const Position& pos, Direction dir, float radius) const;
    bool wouldCollideWithWall(const Position& pos, float radius) const;

    Position getDoorPosition() const { return doorPosition; }
    bool isDoorOpen() const { return doorOpen; }
    bool isInsideSpawn(const Position& pos) const;

private:
    AbstractFactory* factory;

    std::unique_ptr<PacMan> pacman;
    std::vector<std::unique_ptr<Coin>> coins;
    std::vector<std::unique_ptr<Fruit>> fruits;
    std::vector<std::unique_ptr<Wall>> walls;
    std::vector<std::unique_ptr<Ghost>> ghosts;

    int currentLevel = 1;
    Score score;

    Position pacmanSpawnPosition = Position(0, 0);

    float timeSinceLastDeath = 999.0f;
    const float DEATH_COOLDOWN = 2.0f;

    int mapRows = 0;
    int mapCols = 0;

    Position doorPosition;
    bool doorOpen = true;

    // Spawn area bounds
    float spawnLeft = -0.2f;
    float spawnRight = 0.2f;
    float spawnTop = -0.1f;
    float spawnBottom = 0.1f;

    void handleCollisions();
    bool checkWallCollision(const Position& pos, Direction dir) const;
    bool isPositionBlocked(const Position& pos, float radius) const;
    bool isAtIntersection(const Position& pos, Direction currentDir, float radius) const;
    void parseMap(const std::string& mapFile);
    void spawnEntities(const std::vector<std::string>& mapData);
    Position gridToWorld(int row, int col, int totalRows, int totalCols) const;
    void updatePacManWithCollisions(float deltaTime);
    bool checkWallCollision(const Position& pos, float radius) const;
    void applyDifficultyScaling();


};

} // namespace pacman