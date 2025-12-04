#include "representation/ConcreteFactory.h"
#include "representation/views/PacManView.h"

#include "representation/views/CoinView.h"
#include "representation/views/FruitView.h"
#include "representation/views/WallView.h"
#include "representation/views/GhostView.h"

namespace pacman::representation {

ConcreteFactory::ConcreteFactory(const pacman::Camera& camera)
    : camera(camera) {
}

std::unique_ptr<pacman::PacMan> ConcreteFactory::createPacMan(const pacman::Position& pos) {
    auto pacman = std::make_unique<pacman::PacMan>(pos);
    
    // Create and attach view
    auto view = std::make_unique<PacManView>(pacman.get(), camera);
    pacman->attach(view.get());
    views.push_back(std::move(view));
    
    return pacman;
}

std::unique_ptr<pacman::Coin> ConcreteFactory::createCoin(const pacman::Position& pos) {
    auto coin = std::make_unique<pacman::Coin>(pos);
    
    auto view = std::make_unique<CoinView>(coin.get(), camera);
    coin->attach(view.get());
    views.push_back(std::move(view));
    
    return coin;
}

std::unique_ptr<pacman::Fruit> ConcreteFactory::createFruit(const pacman::Position& pos) {
    auto fruit = std::make_unique<pacman::Fruit>(pos);
    
    auto view = std::make_unique<FruitView>(fruit.get(), camera);
    fruit->attach(view.get());
    views.push_back(std::move(view));
    
    return fruit;
}

std::unique_ptr<pacman::Wall> ConcreteFactory::createWall(const pacman::Position& pos) {
    auto wall = std::make_unique<pacman::Wall>(pos);
    
    auto view = std::make_unique<WallView>(wall.get(), camera);
    wall->attach(view.get());
    views.push_back(std::move(view));
    
    return wall;
}

std::unique_ptr<pacman::Ghost> ConcreteFactory::createGhost(const pacman::Position& pos, pacman::GhostColor color) {
    auto ghost = std::make_unique<pacman::Ghost>(pos, color);

    auto view = std::make_unique<GhostView>(ghost.get(), camera, color);
    ghost->attach(view.get());
    views.push_back(std::move(view));

    return ghost;
}

} // namespace pacman::representation