#pragma once
#include "Observer.h"

#include <algorithm>
#include <vector>

namespace pacman {

class Subject {
public:
    virtual ~Subject() = default;
    
    void attach(Observer* observer) {
        observers.push_back(observer);
    }
    
    void detach(Observer* observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }
    
protected:
    void notify(const Event& event) {
        for (auto* observer : observers) {
            observer->onNotify(event);
        }
    }
    
private:
    std::vector<Observer*> observers;  // Raw pointers OK here (non-owning)
};

} // namespace pacman