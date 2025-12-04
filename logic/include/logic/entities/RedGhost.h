#pragma once
#include "logic/entities/Ghost.h"

namespace pacman {

class OrangeGhost : public Ghost {
public:
    explicit OrangeGhost(const Position& pos);
    ~OrangeGhost() override = default;

    Direction chooseDirection(const Position& pacmanPos, Direction pacmanDir) override;
};

} // namespace pacman