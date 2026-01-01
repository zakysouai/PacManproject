#pragma once
#include "logic/EntityModel.h"

namespace pacman {

/**
 * @brief Coin - collectible voor punten
 *
 * - Statisch (beweegt niet)
 * - Base waarde: 10 punten
 * - Combo multiplier: afhankelijk van tijd sinds laatste coin
 */
class Coin : public EntityModel {
public:
    explicit Coin(const Position& pos);
    ~Coin() override = default;

    void update(float deltaTime) override;

    bool isCollected() const { return collected; }
    void collect();

    int getPointValue() const { return pointValue; }
    
private:
    bool collected = false;
    int pointValue = 10;
};

} // namespace pacman