#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <memory>

namespace PacMan {

    /**
     * @brief Singleton class for tracking time between frames
     *
     * Manages deltaTime to ensure consistent game speed across different hardware.
     * Uses C++ chrono instead of SFML for platform independence.
     */
    class Stopwatch {
    public:
        // Singleton pattern
        static std::shared_ptr<Stopwatch> getInstance();

        // Delete copy constructor and assignment operator
        Stopwatch(const Stopwatch&) = delete;
        Stopwatch& operator=(const Stopwatch&) = delete;

        /**
         * @brief Update the stopwatch (call once per frame)
         *
         * Calculates the time difference since last tick
         */
        void tick();

        /**
         * @brief Get time elapsed since last tick
         * @return Delta time in seconds
         */
        float getDeltaTime() const { return m_deltaTime; }

        /**
         * @brief Get total elapsed time since start
         * @return Total time in seconds
         */
        float getTotalTime() const;

        /**
         * @brief Reset the stopwatch
         */
        void reset();

        /**
         * @brief Destructor (public for shared_ptr, but can't be called due to private constructor)
         */
        ~Stopwatch() = default;

    private:
        Stopwatch();

        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = std::chrono::time_point<Clock>;

        static std::shared_ptr<Stopwatch> s_instance;

        TimePoint m_startTime;
        TimePoint m_lastTime;
        float m_deltaTime{0.0f};
    };

} // namespace PacMan

#endif // STOPWATCH_H