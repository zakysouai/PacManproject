// representation/src/ConcreteFactory.cpp
#include "representation/ConcreteFactory.h"
#include "representation/views/PacManView.h"
#include "representation/views/CoinView.h"
#include "representation/views/FruitView.h"
#include "representation/views/WallView.h"
#include "representation/views/GhostView.h"

// ✅ Include concrete ghost types
#include "logic/entities/RedGhost.h"
// TODO: include andere ghost types wanneer ze gemaakt zijn

namespace pacman::representation {

ConcreteFactory::ConcreteFactory(const pacman::Camera& camera)
    : camera(camera) {
}

std::unique_ptr<pacman::PacMan> ConcreteFactory::createPacMan(const pacman::Position& pos) {
    auto pacman = std::make_unique<pacman::PacMan>(pos);

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
    // ✅ Switch op color en maak concrete subclass
    std::unique_ptr<pacman::Ghost> ghost;

    switch (color) {
    case pacman::GhostColor::RED:
        ghost = std::make_unique<pacman::RedGhost>(pos);
        break;
    case pacman::GhostColor::PINK:
        // TODO: ghost = std::make_unique<pacman::PinkGhost>(pos);
        // Tijdelijk: maak red ghost
        ghost = std::make_unique<pacman::RedGhost>(pos);
        break;
    case pacman::GhostColor::BLUE:
        // TODO: ghost = std::make_unique<pacman::BlueGhost>(pos);
        ghost = std::make_unique<pacman::RedGhost>(pos);
        break;
    case pacman::GhostColor::ORANGE:
        // TODO: ghost = std::make_unique<pacman::OrangeGhost>(pos);
        ghost = std::make_unique<pacman::RedGhost>(pos);
        break;
    }

    auto view = std::make_unique<GhostView>(ghost.get(), camera, color);
    ghost->attach(view.get());
    views.push_back(std::move(view));

    return ghost;
}

} // namespace pacman::representation