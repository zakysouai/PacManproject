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

/**
 * @brief Centrale game controller - beheert alle entities en game state
 *
 * Verantwoordelijkheden:
 * - Entity lifecycle (creation, update, destruction)
 * - Collision detection tussen entities
 * - Level loading vanuit text files
 * - Difficulty scaling per level
 * - Game state (level complete, game over)
 *
 * === COORDINATE SYSTEEM ===
 * World gebruikt genormaliseerde coördinaten:
 * - X-as: [-worldWidth, +worldWidth]   (afhankelijk van aspect ratio)
 * - Y-as: [-1.0, +1.0]                 (vast)
 *
 * === COLLISION SYSTEEM ===
 * - Axis-aligned bounding boxes (AABB)
 * - Split X/Y movement (voorkomt diagonal wall clipping)
 * - Corner smoothing (voorkomt vastlopen bij schuin tegen muur)
 * - Door mechanisme (ghosts kunnen 1x door, daarna geblokkeerd)
 *
 * === LEVEL PROGRESSION ===
 * Level N → Level N+1:
 * - Ghost speed: BASE_SPEED * (1.2)^(N-1)  (20% per level)
 * - Fear duration: max(2.0s, 5.0s - 0.5s * N)
 * - Coins/fruits respawnen
 * - Score blijft behouden
 */
class World : public Subject {
public:
    /**
     * @brief Constructor
     * @param factory Reference naar factory voor entity creation
     * @param camera Shared pointer naar camera voor coordinate conversie
     * @param startLevel Initial level (default 1)
     */
    explicit World(AbstractFactory& factory, std::shared_ptr<Camera> camera, int startLevel = 1);
    ~World() = default;

    struct GridPosition {
        int row;
        int col;
    };

    /**
     * @brief Update alle entities en game logica
     * @param deltaTime Tijd sinds laatste frame (seconden)
     *
     * Update volgorde:
     * 1. Death animation (als actief, skip rest)
     * 2. PacMan movement + collision
     * 3. Ghost movement + AI
     * 4. Wraparound check (tunnel edges)
     * 5. Entity updates (notify observers)
     * 6. Collision detection
     */
    void update(float deltaTime);

    /**
     * @brief Laad level vanuit map file
     * @param mapFile Pad naar .txt file (e.g., "../resources/maps/map.txt")
     *
     * Map formaat:
     * - 'x' / 'X': Wall
     * - ' ' / '.': Coin
     * - 'c' / 'C': Fruit
     * - 'p' / 'P': PacMan spawn
     * - 'r' / 'R': Red ghost spawn
     * - 'i' / 'I': Pink ghost spawn
     * - 'b' / 'B': Blue ghost spawn
     * - 'o' / 'O': Orange ghost spawn
     * - 'd' / 'D': Door (ghost spawn exit)
     */
    void loadLevel(const std::string& mapFile);

    void nextLevel();
    void reset();

    // Entity accessors (voor AI en collision)
    PacMan* getPacMan() const { return pacman.get(); }
    const std::vector<std::unique_ptr<Coin>>& getCoins() const { return coins; }
    const std::vector<std::unique_ptr<Fruit>>& getFruits() const { return fruits; }
    const std::vector<std::unique_ptr<Wall>>& getWalls() const { return walls; }
    const std::vector<std::unique_ptr<Ghost>>& getGhosts() const { return ghosts; }

    // Game state
    int getCurrentLevel() const { return currentLevel; }
    bool isLevelComplete() const;
    bool isGameOver() const;

    Score& getScore() { return score; }
    const Score& getScore() const { return score; }

    MapDimensions getMapDimensions() const { return {mapRows, mapCols}; }

    /**
     * @brief Check of entity in direction kan bewegen zonder wall collision
     * @param pos Huidige positie
     * @param dir Te testen direction
     * @param radius Collision radius van entity
     * @param ghost Optional: nullptr voor PacMan, ghost ptr voor door logica
     * @return true als movement geldig is
     *
     * Test positie 0.1 units vooruit in direction.
     * Door blokkeert ghosts die al door zijn geweest.
     */
    bool canMoveInDirection(const Position& pos, Direction dir, float radius, const Ghost* ghost = nullptr) const;

    /**
     * @brief Check of positie botst met wall
     * @param pos Te testen positie
     * @param radius Collision radius
     * @param ghost Optional: voor door blocking logica
     * @return true bij collision
     */
    bool wouldCollideWithWall(const Position& pos, float radius, const Ghost* ghost = nullptr) const;

    // Door mechanisme
    Position getDoorPosition() const { return doorPosition; }
    bool isInsideSpawn(const Position& pos) const;
    bool isDoorPosition(const Position& pos) const;
    bool isDoorBlockingEntity(const Ghost* ghost, const Position& testPos) const;
    bool hasDoorInMap() const { return hasDoor; }

    /**
     * @brief Converteer world position naar grid indices
     * @param worldPos Positie in [-worldWidth, worldWidth] x [-1, 1]
     * @return GridPosition {row, col}
     */
    GridPosition worldToGrid(const Position& worldPos) const;
    GridPosition getDoorGridPosition() const { return doorGridPos; }

    /**
     * @brief Activeer scared mode op alle ghosts
     *
     * Triggered door fruit collection.
     * Duur: fearModeDuration (schaalt per level)
     */
    void activateFearMode();
    float getFearModeDuration() const { return fearModeDuration; }

    bool isPlayingDeathAnimation() const { return deathAnimationPlaying; }

private:
    AbstractFactory& factory;  // Reference - factory moet bestaan tijdens World lifetime
    std::shared_ptr<Camera> camera;

    // Entity containers (ownership via unique_ptr)
    std::unique_ptr<PacMan> pacman;
    std::vector<std::unique_ptr<Coin>> coins;
    std::vector<std::unique_ptr<Fruit>> fruits;
    std::vector<std::unique_ptr<Wall>> walls;
    std::vector<std::unique_ptr<Ghost>> ghosts;

    int currentLevel = 1;
    Score score;

    Position pacmanSpawnPosition = Position(0, 0);

    // Death animation state
    float timeSinceLastDeath = 999.0f;
    const float DEATH_COOLDOWN = 2.0f;
    bool deathAnimationPlaying = false;
    float deathAnimationTimer = 0.0f;
    const float DEATH_ANIMATION_DURATION = 1.2f;

    // Map metadata
    int mapRows = 0;
    int mapCols = 0;
    Position doorPosition;
    bool hasDoor = false;
    GridPosition doorGridPos;

    // Spawn area bounds (voor isInsideSpawn check)
    float spawnLeft = -0.2f;
    float spawnRight = 0.2f;
    float spawnTop = -0.1f;
    float spawnBottom = 0.1f;

    // Difficulty parameters
    float fearModeDuration = 5.0f;

    /**
     * @brief Detecteer en verwerk collisions tussen entities
     *
     * Collision types:
     * - PacMan <-> Coin: collect coin, add points
     * - PacMan <-> Fruit: collect fruit, activate fear mode
     * - PacMan <-> Ghost (normal): lose life, death animation
     * - PacMan <-> Ghost (scared): ghost dies, respawn
     */
    void handleCollisions();

    bool checkWallCollision(const Position& pos, Direction dir) const;
    bool isPositionBlocked(const Position& pos, float radius) const;
    bool isAtIntersection(const Position& pos, Direction currentDir, float radius) const;

    void parseMap(const std::string& mapFile);
    void spawnEntities(const std::vector<std::string>& mapData);

    /**
     * @brief Converteer grid indices naar world position
     * @param row Rij index (0-based)
     * @param col Kolom index (0-based)
     * @param totalRows Totaal aantal rijen in map
     * @param totalCols Totaal aantal kolommen in map
     * @return Position gecentreerd in grid cell
     */
    Position gridToWorld(int row, int col, int totalRows, int totalCols) const;

    /**
     * @brief Update PacMan met collision handling
     *
     * Implementeert:
     * - Input buffering (nextDirection → currentDirection bij intersection)
     * - 180° turns (altijd toegestaan)
     * - Split X/Y movement (voorkomt diagonal clipping)
     * - Corner smoothing (CORNER_SMOOTH offset voor diagonaal glijden)
     */
    void updatePacManWithCollisions(float deltaTime);

    bool checkWallCollision(const Position& pos, float radius) const;

    /**
     * @brief Pas difficulty aan voor huidig level
     *
     * Scaling formules:
     * - Ghost speed: BASE_SPEED * (1.2)^(level-1)
     * - Fear duration: max(2.0, 5.0 - 0.5 * level)
     */
    void applyDifficultyScaling();

    bool isDoorBlockedFor(const Position& pos, const Ghost* ghost) const;

    /**
     * @brief Check tunnel wraparound (edges van map)
     *
     * Als entity buiten bounds gaat, spawn aan andere kant.
     */
    void checkWraparound(EntityModel* entity);

    std::string getMapFileForLevel(int level) const;
};

} // namespace pacman