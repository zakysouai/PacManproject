#pragma once
#include <chrono>

namespace pacman {

/**
 * @brief Singleton Stopwatch - deltaTime voor frame-independent movement
 *
 * === DOEL ===
 * Zorgt voor consistent gameplay ongeacht framerate:
 * - 60 FPS: deltaTime ≈ 0.0167s
 * - 30 FPS: deltaTime ≈ 0.0333s
 * - Movement: distance = speed * deltaTime (constant per seconde)
 *
 * === USAGE ===
 * ```cpp
 * // In Game::run() main loop:
 * Stopwatch::getInstance().update();
 * float dt = Stopwatch::getInstance().getDeltaTime();
 * world.update(dt);
 * ```
 *
 * === SINGLETON PATTERN ===
 * Meyers Singleton (C++11 thread-safe):
 * - Lazy initialization
 * - Automatic destruction
 * - Globally accessible
 */
class Stopwatch {
public:
    /**
     * @brief Verkrijg singleton instance
     * @return Reference naar enige Stopwatch instance
     */
    static Stopwatch& getInstance() {
        static Stopwatch instance;
        return instance;
    }

    // Delete copy/move (singleton)
    Stopwatch(const Stopwatch&) = delete;
    Stopwatch& operator=(const Stopwatch&) = delete;
    Stopwatch(Stopwatch&&) = delete;
    Stopwatch& operator=(Stopwatch&&) = delete;

    /**
     * @brief Verkrijg tijd sinds laatste update
     * @return DeltaTime in seconden
     */
    float getDeltaTime() const { return deltaTime; }

    /**
     * @brief Update stopwatch (roep 1x per frame aan)
     *
     * Berekent verschil tussen huidige tijd en lastTime.
     * Update lastTime voor volgende frame.
     */
    void update() {
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        deltaTime = elapsed.count();
        lastTime = currentTime;
    }

    /**
     * @brief Reset stopwatch naar huidige tijd
     *
     * Gebruikt bij game start om grote deltaTime te voorkomen.
     */
    void reset() {
        lastTime = std::chrono::steady_clock::now();
        deltaTime = 0.0f;
    }

private:
    /**
     * @brief Private constructor (singleton)
     */
    Stopwatch() {
        reset();
    }
    
    std::chrono::time_point<std::chrono::steady_clock> lastTime;
    float deltaTime = 0.0f;
};

} // namespace pacman