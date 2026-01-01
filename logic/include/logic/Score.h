#pragma once
#include "logic/patterns/Observer.h"
#include <vector>
#include <string>

namespace pacman {

/**
 * @brief Highscore entry (naam + score)
 */
struct HighScoreEntry {
    std::string name;
    int score;

    HighScoreEntry() : name("---"), score(0) {}
    HighScoreEntry(const std::string& n, int s) : name(n), score(s) {}
};

/**
 * @brief Score management - punten berekening en highscore persistentie
 *
 * === SCORE SYSTEEM ===
 *
 * Base points:
 * - Coin: 10 pts
 * - Fruit: 50 pts
 * - Ghost: 200 pts
 * - Level complete: 500 * level
 *
 * Combo multiplier (coins):
 * - < 0.5s sinds laatste coin: 3x
 * - 0.5-1.0s sinds laatste coin: 2x
 * - > 1.0s sinds laatste coin: 1x
 *
 * === HIGHSCORE PERSISTENTIE ===
 * - Top 5 scores opgeslagen in text file
 * - Format: "name,score" per regel
 * - Gesorteerd descending
 */
class Score : public Observer {
public:
    Score();
    ~Score() override = default;

    /**
     * @brief Ontvang event en update score
     * @param event Event van Subject (Entity)
     *
     * Event types:
     * - COIN_COLLECTED: addPoints(value * combo)
     * - FRUIT_COLLECTED: addPoints(value)
     * - GHOST_EATEN: addPoints(value)
     * - LEVEL_CLEARED: addPoints(value)
     */
    void onNotify(const Event& event) override;

    int getCurrentScore() const { return currentScore; }
    void addPoints(int points);
    void reset();

    /**
     * @brief Update combo timer
     * @param deltaTime Tijd sinds laatste frame
     *
     * Moet elke frame aangeroepen worden door World.
     */
    void updateComboTimer(float deltaTime);
    float getTimeSinceLastCoin() const { return timeSinceLastCoin; }

    /**
     * @brief Sla highscore op naar file
     * @param playerName Naam van speler
     * @param score Behaalde score
     * @param filename Pad naar highscore file (default: ../resources/highscores/highscores.txt)
     *
     * Voegt entry toe, sorteert, houdt top 5.
     */
    void saveHighScore(const std::string& playerName, int score,
                      const std::string& filename = "../resources/highscores/highscores.txt");

    /**
     * @brief Laad highscores vanuit file
     * @param filename Pad naar highscore file
     *
     * Vult highScores vector, padt aan met "---" entries tot 5.
     */
    void loadHighScores(const std::string& filename = "../resources/highscores/highscores.txt");

    std::vector<HighScoreEntry> getHighScores() const { return highScores; }

    /**
     * @brief Check of score highscore is
     * @param score Te checken score
     * @return true als score > 5e plaats OF < 5 entries
     */
    bool isHighScore(int score) const;

private:
    int currentScore = 0;
    float timeSinceLastCoin = 0.0f;
    std::vector<HighScoreEntry> highScores;  // Top 5 met namen

    /**
     * @brief Bereken combo multiplier op basis van tijd
     * @return 1, 2, of 3
     */
    int calculateComboMultiplier() const;
};

} // namespace pacman