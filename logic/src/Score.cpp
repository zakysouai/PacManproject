#include "logic/Score.h"
#include <fstream>
#include <algorithm>
#include <iostream>

namespace pacman {

Score::Score() {
    loadHighScores();
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
    if (timeSinceLastCoin < 0.5f) return 3;
    if (timeSinceLastCoin < 1.0f) return 2;
    return 1;
}

void Score::reset() {
    currentScore = 0;
    timeSinceLastCoin = 0.0f;
}

bool Score::isHighScore(int score) const {
    if (highScores.size() < 5) return true;
    return score > highScores.back().score;
}

void Score::saveHighScore(const std::string& playerName, int score, const std::string& filename) {

    highScores.emplace_back(playerName, score);

    std::sort(highScores.begin(), highScores.end(),
        [](const HighScoreEntry& a, const HighScoreEntry& b) {
            return a.score > b.score;
        });

    if (highScores.size() > 5) {
        highScores.erase(highScores.begin() + 5, highScores.end());
    }

    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& entry : highScores) {
            file << entry.name << "," << entry.score << "\n";
        }
        file.close();
    } if (!file.is_open()) {
        std::cerr << " FAILED TO OPEN FILE" << std::endl;
        throw std::runtime_error("Failed to save highscore to: " + filename);
    }
}

void Score::loadHighScores(const std::string& filename) {

    highScores.clear();

    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t commaPos = line.find(',');
            if (commaPos != std::string::npos) {
                std::string name = line.substr(0, commaPos);
                int score = std::stoi(line.substr(commaPos + 1));
                highScores.emplace_back(name, score);
            }
        }
    } else {
        std::cerr << "Highscores file couldn't open!" << std::endl;
        throw std::runtime_error("Failed to open highscore file: " + filename);
    }

    while (highScores.size() < 5) {
        highScores.emplace_back("---", 0);
    }
}

} // namespace pacman