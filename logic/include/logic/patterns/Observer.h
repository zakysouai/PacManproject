#pragma once
#include "logic/utils/Types.h"

namespace pacman {

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onNotify(const Event& event) = 0;
};

} // namespace pacman