#pragma once
#include <memory>

namespace pacman {

// Forward declarations
class PacMan;
class Coin;
class Fruit;
class Wall;
class Ghost;
class World;  // ✅ NIEUW
struct Position;

enum class GhostColor;

class AbstractFactory {
public:
    virtual ~AbstractFactory() = default;

    virtual std::unique_ptr<PacMan> createPacMan(const Position& pos) = 0;
    virtual std::unique_ptr<Coin> createCoin(const Position& pos) = 0;
    virtual std::unique_ptr<Fruit> createFruit(const Position& pos) = 0;
    virtual std::unique_ptr<Wall> createWall(const Position& pos) = 0;
    virtual std::unique_ptr<Ghost> createGhost(World& world, const Position& pos, GhostColor color) = 0;  // ✅ World& toegevoegd
};

} // namespace pacman