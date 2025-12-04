#pragma once
#include <memory>

namespace pacman {

// Forward declarations
class PacMan;
class Ghost;
class Coin;
class Fruit;
class Wall;
struct Position;
enum class GhostType;

class AbstractFactory {
public:
    virtual ~AbstractFactory() = default;
    
    // Factory methods for creating entities
    virtual std::unique_ptr<PacMan> createPacMan(const Position& pos) = 0;
    virtual std::unique_ptr<Coin> createCoin(const Position& pos) = 0;
    virtual std::unique_ptr<Fruit> createFruit(const Position& pos) = 0;
    virtual std::unique_ptr<Wall> createWall(const Position& pos) = 0;
};

} // namespace pacman