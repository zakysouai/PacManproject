#pragma once
#include "logic/AbstractFactory.h"
#include "logic/Camera.h"
#include "representation/views/EntityView.h"
#include "logic/entities/Ghost.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace pacman::representation {

/**
 * @brief Concrete implementatie van AbstractFactory
 *
 * === ABSTRACT FACTORY PATTERN ===
 * Doel: Logic library kan entities creëren zonder SFML kennis.
 *
 * Proces:
 * 1. World roept factory->createX(pos) aan
 * 2. ConcreteFactory maakt Model (logic)
 * 3. ConcreteFactory maakt View (representation)
 * 4. View wordt geattached als Observer aan Model
 * 5. View wordt opgeslagen in views vector
 * 6. Model wordt gereturnd naar World (unique_ptr)
 *
 * === VIEW OWNERSHIP ===
 * - Models: owned door World (unique_ptr in vectors)
 * - Views: owned door ConcreteFactory (unique_ptr in views vector)
 * - Observer link: raw pointer (non-owning)
 *
 * === LIFETIME ===
 * ConcreteFactory moet outlive World (anders dangling observers).
 * In LevelState: factory als member, destructor volgorde:
 * 1. ~World() (destroy models, detach observers)
 * 2. ~ConcreteFactory() (destroy views)
 */
class ConcreteFactory : public pacman::AbstractFactory {
public:
    /**
     * @brief Constructor
     * @param camera Shared pointer naar Camera (gedeeld met World en Views)
     */
    explicit ConcreteFactory(std::shared_ptr<pacman::Camera> camera);
    ~ConcreteFactory() override = default;

    // AbstractFactory interface
    std::unique_ptr<pacman::PacMan> createPacMan(const pacman::Position& pos) override;
    std::unique_ptr<pacman::Coin> createCoin(const pacman::Position& pos) override;
    std::unique_ptr<pacman::Fruit> createFruit(const pacman::Position& pos) override;
    std::unique_ptr<pacman::Wall> createWall(const pacman::Position& pos) override;

    /**
     * @brief Creëer ghost met AI type op basis van kleur
     * @param world Reference naar World (ghosts hebben toegang nodig)
     * @param pos Spawn positie
     * @param color Ghost kleur → bepaalt AI subclass
     * @return unique_ptr naar Ghost subclass
     *
     * Color mapping:
     * - RED → RedGhost (direct chaser)
     * - PINK → PinkGhost (predictive)
     * - BLUE → BlueGhost (predictive)
     * - ORANGE → OrangeGhost (random)
     */
    std::unique_ptr<pacman::Ghost> createGhost(pacman::World& world, const pacman::Position& pos,
                                               pacman::GhostColor color) override;

    /**
     * @brief Verkrijg alle views voor rendering
     * @return Const reference naar views vector
     *
     * Gebruikt door LevelState::render() om alle entities te tekenen.
     */
    const std::vector<std::unique_ptr<EntityView>>& getViews() const { return views; }

    /**
     * @brief Verwijder alle views (bij level reset)
     */
    void clearViews() { views.clear(); }

    /**
     * @brief Voeg custom view toe (voor DoorView)
     * @param view unique_ptr naar EntityView subclass
     *
     * Gebruikt voor special cases die niet via create methods gaan.
     */
    void addView(std::unique_ptr<EntityView> view) {
        views.push_back(std::move(view));
    }

private:
    std::weak_ptr<pacman::Camera> camera;  // Weak ptr: factory ownt camera niet
    std::vector<std::unique_ptr<EntityView>> views;  // View ownership

    /**
     * @brief Template helper voor view creation en attachment
     * @tparam ViewType Type van view (PacManView, GhostView, etc.)
     * @tparam ModelType Type van model (PacMan, Ghost, etc.)
     * @param model Raw pointer naar model (non-owning)
     *
     * Niet gebruikt in huidige implementatie (inlined in create methods).
     */
    template<typename ViewType, typename ModelType>
    void attachView(ModelType* model);
};

} // namespace pacman::representation