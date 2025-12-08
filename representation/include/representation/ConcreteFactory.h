#pragma once
#include "logic/AbstractFactory.h"
#include "logic/Camera.h"
#include "representation/views/EntityView.h"
#include "logic/entities/Ghost.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace pacman::representation {

class ConcreteFactory : public pacman::AbstractFactory {
public:
    explicit ConcreteFactory(std::shared_ptr<pacman::Camera> camera);
    ~ConcreteFactory() override = default;

    std::unique_ptr<pacman::PacMan> createPacMan(const pacman::Position& pos) override;
    std::unique_ptr<pacman::Coin> createCoin(const pacman::Position& pos) override;
    std::unique_ptr<pacman::Fruit> createFruit(const pacman::Position& pos) override;
    std::unique_ptr<pacman::Wall> createWall(const pacman::Position& pos) override;
    std::unique_ptr<pacman::Ghost> createGhost(pacman::World& world, const pacman::Position& pos, pacman::GhostColor color) override;  // ✅ World& toegevoegd

    const std::vector<std::unique_ptr<EntityView>>& getViews() const { return views; }
    void clearViews() { views.clear(); }

    void addView(std::unique_ptr<EntityView> view) {
        views.push_back(std::move(view));
    }

private:
    std::weak_ptr<pacman::Camera> camera;
    std::vector<std::unique_ptr<EntityView>> views;

    template<typename ViewType, typename ModelType>
    void attachView(ModelType* model);
};

} // namespace pacman::representation