#pragma once
#include <chrono>

namespace pacman {

class Stopwatch {
public:
    // Singleton access
    static Stopwatch& getInstance() {
        static Stopwatch instance;
        return instance;
    }
    
    // Delete copy and move
    Stopwatch(const Stopwatch&) = delete;
    Stopwatch& operator=(const Stopwatch&) = delete;
    Stopwatch(Stopwatch&&) = delete;
    Stopwatch& operator=(Stopwatch&&) = delete;
    
    // Get delta time in seconds
    float getDeltaTime() const { return deltaTime; }
    
    // Update the stopwatch (call once per frame)
    void update() {
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        deltaTime = elapsed.count();
        lastTime = currentTime;
    }
    
    // Reset the stopwatch
    void reset() {
        lastTime = std::chrono::steady_clock::now();
        deltaTime = 0.0f;
    }
    
private:
    Stopwatch() {
        reset();
    }
    
    std::chrono::time_point<std::chrono::steady_clock> lastTime;
    float deltaTime = 0.0f;
};

} // namespace pacman