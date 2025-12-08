#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

class BlueGhost : public Ghost {
public:
    explicit BlueGhost(World& world, const Position& pos);  // ✅ World& toegevoegd
    ~BlueGhost() override = default;

protected:
    Direction chooseDirection() override;

private:
    const float PREDICT_DISTANCE = 0.4f;
};

} // namespace pacman