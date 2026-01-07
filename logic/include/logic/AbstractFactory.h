 #pragma once
#include <memory>

namespace pacman {

// Forward declarations
class PacMan;
class Coin;
class Fruit;
class Wall;
class Ghost;
class World;
struct Position;

enum class GhostColor;

/**
 * @brief Abstract Factory pattern - interface voor entity creation
 *
 * === DOEL ===
 * Scheiding tussen logic en representation:
 * - Logic library definieert interface (AbstractFactory)
 * - Representation library implementeert (ConcreteFactory)
 * - World gebruikt factory zonder SFML kennis
 *
 * === USAGE ===
 * 1. ConcreteFactory extends AbstractFactory
 * 2. Implementeert create methods (maakt Entity + View, attach observer)
 * 3. World.loadLevel() gebruikt factory->createX() voor spawning
 *
 * === OBSERVER ATTACHMENT ===
 * Factory is verantwoordelijk voor:
 * - Creëren van Model (unique_ptr)
 * - Creëren van View (stored in ConcreteFactory)
 * - Attachen van View als Observer aan Model
 */
class AbstractFactory {
public:
    virtual ~AbstractFactory() = default;

    virtual std::unique_ptr<PacMan> createPacMan(const Position& pos) = 0;
    virtual std::unique_ptr<Coin> createCoin(const Position& pos) = 0;
    virtual std::unique_ptr<Fruit> createFruit(const Position& pos) = 0;
    virtual std::unique_ptr<Wall> createWall(const Position& pos) = 0;

    /**
     * @brief Creëer ghost met specifieke AI
     * @param world Reference naar World (ghosts hebben toegang nodig voor pathfinding)
     * @param pos Spawn positie
     * @param color Ghost kleur (bepaalt AI type en visual)
     * @return unique_ptr naar Ghost subclass
     */
    virtual std::unique_ptr<Ghost> createGhost(World& world, const Position& pos, GhostColor color) = 0;
};

} // namespace pacman