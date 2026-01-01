#pragma once
#include "logic/EntityModel.h"
#include "logic/World.h"

namespace pacman {

enum class GhostColor {
    RED,
    PINK,
    BLUE,
    ORANGE
};

/**
 * @brief Ghost state machine
 *
 * - IN_SPAWN: Wacht in spawn area, beweegt niet
 * - ON_MAP: Actief, jaagt PacMan (normale AI)
 * - SCARED: Vlucht van PacMan, kan gegeten worden
 */
enum class GhostState {
    IN_SPAWN,
    ON_MAP,
    SCARED
};

/**
 * @brief Abstracte basisklasse voor ghost AI
 *
 * Ghosts hebben verschillende AI strategieën (geïmplementeerd in subklassen):
 * - RedGhost: Direct chaser (minimaliseer Manhattan distance)
 * - PinkGhost: Predictive (target 4 tiles voor PacMan)
 * - BlueGhost: Predictive (target 4 tiles voor PacMan)
 * - OrangeGhost: Random movement met 50% kans op direction change
 *
 * === STATE FLOW ===
 * IN_SPAWN → (spawn delay) → ON_MAP → (fruit eaten) → SCARED → ON_MAP
 *                                  ↓ (eaten by PacMan)
 *                                 respawn (direct ON_MAP, geen delay)
 *
 * === MOVEMENT LOGIC ===
 * - Alleen direction changes bij intersections (3+ viable directions)
 * - Geen 180° turns (kan niet omdraaien)
 * - Door blokkering: ghosts kunnen 1x door, daarna geblokkeerd
 */
class Ghost : public EntityModel {
public:
    /**
     * @brief Constructor
     * @param world Reference naar World (voor pathfinding en PacMan positie)
     * @param pos Spawn positie (center van map)
     * @param color Ghost kleur (voor visuals)
     * @param spawnDelay Tijd in seconden voordat ghost spawn verlaat
     */
    explicit Ghost(World& world, const Position& pos, GhostColor color, float spawnDelay);
    virtual ~Ghost() = default;

    void update(float deltaTime) override;

    // State getters
    GhostColor getColor() const { return color; }
    GhostState getState() const { return state; }
    Direction getCurrentDirection() const { return currentDirection; }
    float getScaredTimeRemaining() const { return scaredTimer; }

    // Difficulty scaling
    void setNormalSpeed(float speed) { normalSpeed = speed; }

    // Door passage tracking (ghosts kunnen 1x door, daarna geblokkeerd)
    bool hasPassedThroughDoor() const { return hasPassedDoor; }
    void markPassedDoor() { hasPassedDoor = true; }

    /**
     * @brief Activeer scared mode
     * @param duration Duur in seconden
     *
     * - Speed wordt gehalveerd
     * - AI maximaliseert distance (vlucht van PacMan)
     * - Direction wordt omgedraaid
     */
    void enterScaredMode(float duration);

    bool isScared() const { return state == GhostState::SCARED; }

    /**
     * @brief Respawn na opgegeten te zijn
     *
     * Direct terug op map (GEEN spawn delay).
     * Gebruikt bij death door PacMan.
     */
    void respawn();

    /**
     * @brief Reset naar spawn positie
     *
     * Met spawn delay. Gebruikt bij level start of PacMan death.
     */
    void reset();

    void die();

protected:
    /**
     * @brief Pure virtual: AI strategy voor direction keuze
     * @return Nieuwe direction op basis van AI logica
     *
     * Wordt ALLEEN aangeroepen bij intersections.
     * Moet rekening houden met:
     * - Geen 180° turns
     * - Viable directions (geen muren)
     * - AI-specifieke target (PacMan positie, prediction, random)
     */
    virtual Direction chooseDirection() = 0;

    /**
     * @brief Check of ghost op intersection is
     * @return true als 3+ viable directions beschikbaar
     */
    bool isAtIntersection() const;

    /**
     * @brief Verkrijg alle geldige directions (excl. opposite)
     * @return Vector van viable directions
     */
    std::vector<Direction> getViableDirections() const;

    /**
     * @brief Vind beste direction naar target
     * @param target Doel positie
     * @param maximize false = minimize distance (chase), true = maximize (flee)
     * @return Direction die Manhattan distance minimaliseert/maximaliseert
     */
    Direction getBestDirectionToTarget(const Position& target, bool maximize = false) const;

    World& world;  // Reference niet pointer - ghost MOET valid world hebben
    Direction currentDirection = Direction::RIGHT;

private:
    GhostColor color;
    GhostState state = GhostState::IN_SPAWN;
    Position spawnPosition;

    float spawnTimer = 0.0f;
    bool hasPassedDoor = false;

    float scaredTimer = 0.0f;
    float normalSpeed = 0.15f;
    GhostState previousState = GhostState::ON_MAP;
    float initialSpawnDelay = 0.0f;

    void move(float deltaTime);
    void handleWallCollision();
    bool isOpposite(Direction dir1, Direction dir2) const;
    float calculateManhattanDistance(const Position& from, const Position& to) const;
    Direction chooseDirectionAtIntersection();
    Direction getOppositeDirection(Direction dir) const;
};

} // namespace pacman