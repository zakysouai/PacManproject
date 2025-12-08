#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

class OrangeGhost : public Ghost {
public:
    explicit OrangeGhost(World& world, const Position& pos);  // ✅ World& toegevoegd
    ~OrangeGhost() override = default;

protected:
    Direction chooseDirection() override;

private:
    Direction lockedDirection = Direction::RIGHT;
};

} // namespace pacman