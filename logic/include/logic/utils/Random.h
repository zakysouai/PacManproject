#pragma once
#include <random>

namespace pacman {

/**
 * @brief Singleton Random - pseudo-random number generator
 *
 * === GEBRUIK ===
 * - Ghost AI (OrangeGhost random direction)
 * - Tie breaking (meerdere beste directions)
 * - Map generation (bonus: procedural maps)
 *
 * === MERSENNE TWISTER ===
 * std::mt19937: High-quality PRNG (2^19937-1 period)
 * Seeded met std::random_device (hardware entropy)
 *
 * === SINGLETON PATTERN ===
 * Voorkomt re-seeding bij elke random call.
 * Eén generator = betere randomness.
 */
class Random {
public:
    /**
     * @brief Verkrijg singleton instance
     * @return Reference naar enige Random instance
     */
    static Random& getInstance() {
        static Random instance;
        return instance;
    }

    // Delete copy/move (singleton)
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;
    Random(Random&&) = delete;
    Random& operator=(Random&&) = delete;

    /**
     * @brief Random integer in range [min, max] (inclusive)
     * @param min Minimum waarde
     * @param max Maximum waarde
     * @return Random int
     *
     * Gebruikt voor:
     * - Direction index (0-3 voor UP/DOWN/LEFT/RIGHT)
     * - Tie breaking tussen viable directions
     */
    int getInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(generator);
    }

    /**
     * @brief Random float in range [min, max]
     * @param min Minimum waarde
     * @param max Maximum waarde
     * @return Random float
     */
    float getFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(generator);
    }

    /**
     * @brief Random boolean met gegeven probability
     * @param probability Kans op true (0.0-1.0)
     * @return true met probability p, false met 1-p
     *
     * Gebruikt voor OrangeGhost (50% kans op direction change).
     */
    bool getBool(float probability = 0.5f) {
        std::bernoulli_distribution dist(probability);
        return dist(generator);
    }

private:
    /**
     * @brief Private constructor (singleton)
     *
     * Seed generator met hardware random device.
     */
    Random() {
        std::random_device rd;
        generator.seed(rd());
    }

    std::mt19937 generator;  // Mersenne Twister PRNG
};

} // namespace pacman