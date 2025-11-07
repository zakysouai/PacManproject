#pragma once
#include "logic/patterns/Observer.h"
#include <vector>
#include <string>

namespace pacman {

class Score : public Observer {
public:
    Score();
    ~Score() override = default;
    
    // Observer interface
    void onNotify(const Event& event) override;
    
    // Score management
    int getCurrentScore() const { return currentScore; }
    void addPoints(int points);
    void reset();
    
    // Combo system (for time-based scoring)
    void updateComboTimer(float deltaTime);
    float getTimeSinceLastCoin() const { return timeSinceLastCoin; }
    
    // High scores
    void saveHighScores(const std::string& filename);
    void loadHighScores(const std::string& filename);
    std::vector<int> getHighScores() const { return highScores; }
    bool isHighScore(int score) const;
    
private:
    int currentScore = 0;
    float timeSinceLastCoin = 0.0f;
    std::vector<int> highScores;  // Top 5
    
    // Calculate score modifier based on timing
    int calculateComboMultiplier() const;
};

} // namespace pacman