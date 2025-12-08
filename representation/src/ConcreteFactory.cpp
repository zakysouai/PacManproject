#include "representation/ConcreteFactory.h"
#include "representation/views/PacManView.h"
#include "representation/views/CoinView.h"
#include "representation/views/FruitView.h"
#include "representation/views/WallView.h"
#include "representation/views/GhostView.h"

#include "logic/entities/RedGhost.h"
#include "logic/entities/PinkGhost.h"
#include "logic/entities/BlueGhost.h"
#include "logic/entities/OrangeGhost.h"

namespace pacman::representation {

ConcreteFactory::ConcreteFactory(std::shared_ptr<pacman::Camera> camera)  // ✅ CHANGED
    : camera(camera) {
}

std::unique_ptr<pacman::PacMan> ConcreteFactory::createPacMan(const pacman::Position& pos) {
    auto pacman = std::make_unique<pacman::PacMan>(pos);

    auto view = std::make_unique<PacManView>(*pacman, camera);
    pacman->attach(view.get());
    views.push_back(std::move(view));

    return pacman;
}

std::unique_ptr<pacman::Coin> ConcreteFactory::createCoin(const pacman::Position& pos) {
    auto coin = std::make_unique<pacman::Coin>(pos);
    auto view = std::make_unique<CoinView>(*coin, camera);  // *coin geeft reference
    coin->attach(view.get());
    views.push_back(std::move(view));
    return coin;
}

std::unique_ptr<pacman::Fruit> ConcreteFactory::createFruit(const pacman::Position& pos) {
    auto fruit = std::make_unique<pacman::Fruit>(pos);

    auto view = std::make_unique<FruitView>(*fruit, camera);  // ✅ Pass weak_ptr
    fruit->attach(view.get());
    views.push_back(std::move(view));

    return fruit;
}

std::unique_ptr<pacman::Wall> ConcreteFactory::createWall(const pacman::Position& pos) {
    auto wall = std::make_unique<pacman::Wall>(pos);

    auto view = std::make_unique<WallView>(*wall, camera);  // ✅ Pass weak_ptr
    wall->attach(view.get());
    views.push_back(std::move(view));

    return wall;
}

std::unique_ptr<pacman::Ghost> ConcreteFactory::createGhost(const pacman::Position& pos, pacman::GhostColor color) {
    std::unique_ptr<pacman::Ghost> ghost;

    switch (color) {
    case pacman::GhostColor::RED:
        ghost = std::make_unique<pacman::RedGhost>(pos);
        break;
    case pacman::GhostColor::PINK:
        ghost = std::make_unique<pacman::PinkGhost>(pos);
        break;
    case pacman::GhostColor::BLUE:
        ghost = std::make_unique<pacman::BlueGhost>(pos);
        break;
    case pacman::GhostColor::ORANGE:
        ghost = std::make_unique<pacman::OrangeGhost>(pos);
        break;
    }

    auto view = std::make_unique<GhostView>(*ghost, camera, color);  // ✅ Pass weak_ptr
    ghost->attach(view.get());
    views.push_back(std::move(view));

    return ghost;
}

} // namespace pacman::representation