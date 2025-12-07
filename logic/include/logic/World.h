#pragma once
#include "Camera.h"
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

class World : public Subject {
public:
    explicit World(AbstractFactory* factory);
    ~World() = default;

    struct GridPosition {
        int row;
        int col;
    };
    
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
    void setCamera(Camera* cam) { camera = cam; }

    bool canMoveInDirection(const Position& pos, Direction dir, float radius, const Ghost* ghost = nullptr) const;
    bool wouldCollideWithWall(const Position& pos, float radius, const Ghost* ghost = nullptr) const;

    Position getDoorPosition() const { return doorPosition; }
    bool isInsideSpawn(const Position& pos) const;

    bool isDoorPosition(const Position& pos) const;
    bool isDoorBlockingEntity(const Ghost* ghost, const Position& testPos) const;
    bool hasDoorInMap() const{ return hasDoor; }

    GridPosition worldToGrid(const Position& worldPos) const;
    GridPosition getDoorGridPosition() const { return doorGridPos; }

    void activateFearMode();
    float getFearModeDuration() const { return fearModeDuration; }

    bool isPlayingDeathAnimation() const { return deathAnimationPlaying; }

private:
    AbstractFactory* factory;
    Camera* camera = nullptr;

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
    bool deathAnimationPlaying = false;
    float deathAnimationTimer = 0.0f;
    const float DEATH_ANIMATION_DURATION = 1.2f;

    Position doorPosition;
    bool hasDoor = false;  // Was er een 'D' in de map?
    GridPosition doorGridPos;

    // Spawn area bounds
    float spawnLeft = -0.2f;
    float spawnRight = 0.2f;
    float spawnTop = -0.1f;
    float spawnBottom = 0.1f;

    float fearModeDuration = 5.0f;  // Base duration, scaled per level

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
    bool isDoorBlockedFor(const Position& pos, const Ghost* ghost) const;
    void checkWraparound(EntityModel* entity);
    std::string getMapFileForLevel(int level) const;
};

} // namespace pacman