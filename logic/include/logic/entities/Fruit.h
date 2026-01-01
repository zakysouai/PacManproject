#pragma once
#include "logic/EntityModel.h"

namespace pacman {

/**
 * @brief Fruit - power-up voor scared mode
 *
 * - Statisch (spawn in hoeken van map)
 * - Waarde: 50 punten
 * - Effect: Activeer scared mode op alle ghosts
 */
class Fruit : public EntityModel {
public:
    explicit Fruit(const Position& pos);
    ~Fruit() override = default;

    void update(float deltaTime) override;

    bool isCollected() const { return collected; }
    void collect();

    int getPointValue() const { return pointValue; }
    
private:
    bool collected = false;
    int pointValue = 50;
};

} // namespace pacman