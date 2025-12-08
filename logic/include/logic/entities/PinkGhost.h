#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

class PinkGhost : public Ghost {
public:
    explicit PinkGhost(World& world, const Position& pos);  // ✅ World& toegevoegd
    ~PinkGhost() override = default;

protected:
    Direction chooseDirection() override;

private:
    const float PREDICT_DISTANCE = 0.4f;
};

} // namespace pacman