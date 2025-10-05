#include "Stopwatch.h"

namespace PacMan {

    // Initialize static member
    std::shared_ptr<Stopwatch> Stopwatch::s_instance = nullptr;

    Stopwatch::Stopwatch()
        : m_startTime(Clock::now()), m_lastTime(Clock::now()) {
    }

    std::shared_ptr<Stopwatch> Stopwatch::getInstance() {
        if (!s_instance) {
            // Can't use make_shared due to private constructor
            s_instance = std::shared_ptr<Stopwatch>(new Stopwatch());
        }
        return s_instance;
    }

    void Stopwatch::tick() {
        TimePoint currentTime = Clock::now();

        // Calculate delta time in seconds
        std::chrono::duration<float> elapsed = currentTime - m_lastTime;
        m_deltaTime = elapsed.count();

        // Update last time
        m_lastTime = currentTime;
    }

    float Stopwatch::getTotalTime() const {
        TimePoint currentTime = Clock::now();
        std::chrono::duration<float> elapsed = currentTime - m_startTime;
        return elapsed.count();
    }

    void Stopwatch::reset() {
        m_startTime = Clock::now();
        m_lastTime = m_startTime;
        m_deltaTime = 0.0f;
    }

} // namespace PacMan