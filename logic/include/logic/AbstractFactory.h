#pragma once
#include <memory>

namespace pacman {

// Forward declarations
class PacMan;
class Coin;
class Fruit;
class Wall;
class Ghost;
struct Position;

// ✅ VOEG TOE: GhostColor enum
enum class GhostColor;  // Forward declaration

class AbstractFactory {
public:
    virtual ~AbstractFactory() = default;

    virtual std::unique_ptr<PacMan> createPacMan(const Position& pos) = 0;
    virtual std::unique_ptr<Coin> createCoin(const Position& pos) = 0;
    virtual std::unique_ptr<Fruit> createFruit(const Position& pos) = 0;
    virtual std::unique_ptr<Wall> createWall(const Position& pos) = 0;
    virtual std::unique_ptr<Ghost> createGhost(const Position& pos, GhostColor color) = 0;
};

} // namespace pacman