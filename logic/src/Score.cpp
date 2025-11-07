#include "logic/Score.h"
#include "logic/utils/Stopwatch.h"
#include <fstream>
#include <algorithm>

namespace pacman {

Score::Score() {
    highScores = {0, 0, 0, 0, 0};  // Initialize top 5
}

void Score::onNotify(const Event& event) {
    switch (event.type) {
    case EventType::COIN_COLLECTED:
        addPoints(event.value * calculateComboMultiplier());
        timeSinceLastCoin = 0.0f;
        break;

    case EventType::FRUIT_COLLECTED:
        addPoints(event.value);
        break;

    case EventType::GHOST_EATEN:
        addPoints(event.value);
        break;

    case EventType::LEVEL_CLEARED:
        addPoints(event.value);
        break;

    default:
        break;
    }
}

void Score::addPoints(int points) {
    currentScore += points;
}

void Score::updateComboTimer(float deltaTime) {
    timeSinceLastCoin += deltaTime;
}

int Score::calculateComboMultiplier() const {
    // Fast combo = higher multiplier
    if (timeSinceLastCoin < 0.5f) return 3;
    if (timeSinceLastCoin < 1.0f) return 2;
    return 1;
}

void Score::reset() {
    currentScore = 0;
    timeSinceLastCoin = 0.0f;
}

bool Score::isHighScore(int score) const {
    return score > highScores.back();
}

void Score::saveHighScores(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int score : highScores) {
            file << score << "\n";
        }
    }
}

void Score::loadHighScores(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        highScores.clear();
        int score;
        while (file >> score) {
            highScores.push_back(score);
        }
        // Ensure we have exactly 5 scores
        while (highScores.size() < 5) {
            highScores.push_back(0);
        }
    }
}

} // namespace pacman