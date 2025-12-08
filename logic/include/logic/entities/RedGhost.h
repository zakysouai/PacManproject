#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

class RedGhost : public Ghost {
public:
    explicit RedGhost(World& world, const Position& pos);  // ✅ World& toegevoegd
    ~RedGhost() override = default;

protected:
    Direction chooseDirection() override;
};

} // namespace pacman