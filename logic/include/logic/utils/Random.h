#pragma once
#include <random>

namespace pacman {

class Random {
public:
    // Singleton access
    static Random& getInstance() {
        static Random instance;
        return instance;
    }
    
    // Delete copy and move
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;
    Random(Random&&) = delete;
    Random& operator=(Random&&) = delete;
    
    // Get random integer in range [min, max]
    int getInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(generator);
    }
    
    // Get random float in range [min, max]
    float getFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(generator);
    }
    
    // Get random bool with probability p
    bool getBool(float probability = 0.5f) {
        std::bernoulli_distribution dist(probability);
        return dist(generator);
    }
    
private:
    Random() {
        std::random_device rd;
        generator.seed(rd());
    }
    
    std::mt19937 generator;
};

} // namespace pacman