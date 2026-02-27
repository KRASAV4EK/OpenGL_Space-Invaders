#pragma once

#include <string>

// ------------------------------------------------------------------------------------------------
// Score
//
// Static utility class responsible for tracking, displaying, saving, and loading
// both the current session score and the persistent highest score.
//
// The class stores the score as a simple integer value in the range 0–999.
// Rendering uses the number sprites loaded in GameSettings (3-digit display).
//
// All methods and data members are static — no instances of Score are created.
// ------------------------------------------------------------------------------------------------
class Score
{
public:
    // Saves the highest score to persistent storage (text file).
    static void Save();

    // Loads the highest score from file on startup.
    static void Load();

    // Resets the current score back to zero.
    static void Reset();

    // Adds points to the current score.
    static void Add(int value);

    // Returns the current or highest scores.
    static int GetCurrent();
    static int GetHighest();

    // Updates highest score if the current session exceeded it.
    // Resets current score afterwards.
    static void UpdateHighest();

    // Renders both the current and highest score numeric icons on screen.
    static void UpdateIcons();

private:
    // Path where the highscore is saved.
    static const std::string SCORE_PATH;

    // Runtime values: score in current game and highest ever stored.
    static int currentScore, highestScore;

    // Cached drawing positions for score rendering (loaded from GameSettings).
    static float currentX, currentY;
    static float highestX, highestY;

    // Converts a 3-digit integer into individual indexes (hundreds, tens, ones).
    static void GetScoreIndexes(bool takeHighest, int& first, int& second, int& third);

    // Draws the current score at the UI location.
    static void UpdateCurrentScore();

    // Draws the highest score at the UI location.
    static void UpdateHighestScore();
};
