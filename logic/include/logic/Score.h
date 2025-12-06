#pragma once
#include "logic/patterns/Observer.h"
#include <vector>
#include <string>

namespace pacman {

struct HighScoreEntry {
    std::string name;
    int score;

    HighScoreEntry() : name("---"), score(0) {}  // ✅ DEFAULT CONSTRUCTOR
    HighScoreEntry(const std::string& n, int s) : name(n), score(s) {}
};

class Score : public Observer {
public:
    Score();
    ~Score() override = default;

    void onNotify(const Event& event) override;

    int getCurrentScore() const { return currentScore; }
    void addPoints(int points);
    void reset();

    void updateComboTimer(float deltaTime);
    float getTimeSinceLastCoin() const { return timeSinceLastCoin; }

    // ✅ NIEUWE HIGHSCORE FUNCTIES
    void saveHighScore(const std::string& playerName, int score, const std::string& filename = "../resources/highscores/highscores.txt");
    void loadHighScores(const std::string& filename = "../resources/highscores/highscores.txt");
    std::vector<HighScoreEntry> getHighScores() const { return highScores; }
    bool isHighScore(int score) const;

private:
    int currentScore = 0;
    float timeSinceLastCoin = 0.0f;
    std::vector<HighScoreEntry> highScores;  // ✅ Top 5 met namen

    int calculateComboMultiplier() const;
};

} // namespace pacman