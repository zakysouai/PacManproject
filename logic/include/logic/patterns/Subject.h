#pragma once
#include "Observer.h"

#include <algorithm>
#include <vector>

namespace pacman {

/**
 * @brief Observer pattern - verzender van events
 *
 * Subjects (EntityModel, World) notificeren Observers bij state changes.
 *
 * === GEBRUIK ===
 * 1. Observer attach bij creation (in ConcreteFactory)
 * 2. Subject::notify() bij relevante state changes
 * 3. Observer detach bij destruction (automatic in ~EntityView)
 */
class Subject {
public:
    virtual ~Subject() = default;

    /**
     * @brief Registreer observer
     * @param observer Raw pointer (non-owning)
     *
     * Observer lifecycle wordt NIET beheerd door Subject.
     * Observer moet zelf detach in destructor.
     */
    void attach(Observer* observer) {
        observers.push_back(observer);
    }

    /**
     * @brief Deregistreer observer
     * @param observer Te verwijderen observer
     */
    void detach(Observer* observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }

protected:
    /**
     * @brief Stuur event naar alle observers
     * @param event Event data
     */
    void notify(const Event& event) {
        for (auto* observer : observers) {
            observer->onNotify(event);
        }
    }

private:
    std::vector<Observer*> observers;  // Raw pointers OK (non-owning)
};

} // namespace pacman