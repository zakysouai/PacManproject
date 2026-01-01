#pragma once
#include "logic/utils/Types.h"

namespace pacman {

/**
 * @brief Observer pattern - ontvanger van events
 *
 * Implementaties:
 * - EntityView: Update visuals bij Model changes
 * - Score: Update score bij COIN_COLLECTED, etc.
 */
class Observer {
public:
    virtual ~Observer() = default;

    /**
     * @brief Ontvang event van Subject
     * @param event Event data (type, value, deltaTime)
     */
    virtual void onNotify(const Event& event) = 0;
};

} // namespace pacman