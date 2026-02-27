#include "../Headers/Score.h"
#include "../Headers/GameSettings.h"
#include "../lib/leetlib.h"

#include <filesystem>
#include <fstream>
#include <limits>

// ------------------------------------------------------------------------------------------------
// Static member initializations
// ------------------------------------------------------------------------------------------------
const std::string Score::SCORE_PATH = "Highscore/Score.txt";

int Score::currentScore = 0;
int Score::highestScore = 0;

float Score::currentX = Settings.SCORE_CURRENT_X;
float Score::currentY = Settings.SCORE_CURRENT_Y;

float Score::highestX = Settings.SCORE_HIGHEST_X;
float Score::highestY = Settings.SCORE_HIGHEST_Y;

// ------------------------------------------------------------------------------------------------
// File writing helper — ensures parent folder exists
// Returns true when successful, false on error
// ------------------------------------------------------------------------------------------------
static inline bool WriteTextFile(const std::string& path, const std::string& text)
{
    namespace fs = std::filesystem;
    try {
        if (auto parent = fs::path(path).parent_path(); !parent.empty())
            fs::create_directories(parent);

        std::ofstream out(path, std::ios::trunc);
        if (!out) return false;

        out << text;
        return out.good();
    }
    catch (...) {
        return false;
    }
}

// ------------------------------------------------------------------------------------------------
// File reading helper — loads file content into outText
// Returns true on success
// ------------------------------------------------------------------------------------------------
static inline bool ReadTextFile(const std::string& path, std::string& outText)
{
    try {
        std::ifstream in(path);
        if (!in) return false;

        std::string data(
            (std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());

        outText = std::move(data);
        return true;
    }
    catch (...) {
        return false;
    }
}

// ------------------------------------------------------------------------------------------------
// Saves the highest score to SCORE_PATH as text
// ------------------------------------------------------------------------------------------------
void Score::Save()
{
    UpdateHighest();
    WriteTextFile(SCORE_PATH, std::to_string(highestScore));
}

// ------------------------------------------------------------------------------------------------
// Loads the highest score from SCORE_PATH
// Sanitizes the file contents: trims whitespace and prevents negative values
// On any error: sets score to 0 and prints message
// ------------------------------------------------------------------------------------------------
void Score::Load()
{
    std::string text;
    if (!ReadTextFile(SCORE_PATH, text)) {
        highestScore = 0;
        std::cerr << "Unable to open Score file: \"" << SCORE_PATH
            << "\". Starting with 0 highscore...";
        return;
    }

    try {
        // Trim leading/trailing whitespace
        size_t start = text.find_first_not_of(" \t\r\n");
        size_t end = text.find_last_not_of(" \t\r\n");

        if (start == std::string::npos) {
            highestScore = 0;
            return;
        }

        std::string trimmed = text.substr(start, end - start + 1);

        long long val = std::stoll(trimmed);

        if (val < 0) {
            highestScore = 0;
            std::cerr << "Unable to read data from Score file: \"" << SCORE_PATH
                << "\". Starting with 0 highscore...";
            return;
        }

        highestScore = static_cast<int>(val);
        std::cerr << "Highscore successfully loaded from Score file: \"" << SCORE_PATH
            << "\". Starting with " << highestScore << " highscore...";
    }
    catch (...) {
        highestScore = 0;
        std::cerr << "Error reading data from Score file: \"" << SCORE_PATH
            << "\". Starting with 0 highscore...";
    }
}

// ------------------------------------------------------------------------------------------------
// Clears the current score for a new session
// ------------------------------------------------------------------------------------------------
void Score::Reset()
{
    currentScore = 0;
}

// ------------------------------------------------------------------------------------------------
// Adds points to the current score (if negative, this simply subtracts)
// ------------------------------------------------------------------------------------------------
void Score::Add(int value)
{
    currentScore += value;
}

int Score::GetCurrent() { return currentScore; }
int Score::GetHighest() { return highestScore; }

// ------------------------------------------------------------------------------------------------
// Updates highest score if needed and resets current score afterward
// ------------------------------------------------------------------------------------------------
void Score::UpdateHighest()
{
    highestScore = highestScore > currentScore ? highestScore : currentScore;
    Reset();
}

// ------------------------------------------------------------------------------------------------
// Extracts digits (hundreds, tens, units) from current or highest score
// Stored in integers for selecting correct number sprites
// ------------------------------------------------------------------------------------------------
void Score::GetScoreIndexes(bool takeHighest, int& first, int& second, int& third)
{
    int score = takeHighest ? highestScore : currentScore;

    first = score / 100;
    second = (score / 10) % 10;
    third = score % 10;
}

// ------------------------------------------------------------------------------------------------
// Draws the current score (3 digits) using number sprites
// ------------------------------------------------------------------------------------------------
void Score::UpdateCurrentScore()
{
    int first = 0, second = 0, third = 0;
    GetScoreIndexes(false, first, second, third);

    for (int i = 0; i < 3; i++)
    {
        DrawSprite(
            Settings.spritesNumbers[
                (i == 0 ? first : (i == 1 ? second : third))
            ],
            Settings.SCORE_CURRENT_X + Settings.SCORE_CURRENT_X_OFFSET * i,
            Settings.SCORE_CURRENT_Y,
            Settings.SCORE_CURRENT_SIZE,
            Settings.SCORE_CURRENT_SIZE,
            0
        );
    }
}

// ------------------------------------------------------------------------------------------------
// Draws the highest score (3 digits)
// Clamps max displayed score to 999 to avoid array overflow
// ------------------------------------------------------------------------------------------------
void Score::UpdateHighestScore()
{
    if (highestScore > 999)
        highestScore = 999;

    int first = 0, second = 0, third = 0;
    GetScoreIndexes(true, first, second, third);

    for (int i = 0; i < 3; i++)
    {
        DrawSprite(
            Settings.spritesNumbers[
                (i == 0 ? first : (i == 1 ? second : third))
            ],
            Settings.SCORE_HIGHEST_X + Settings.SCORE_HIGHEST_X_OFFSET * i,
            Settings.SCORE_HIGHEST_Y,
            Settings.SCORE_HIGHEST_SIZE,
            Settings.SCORE_HIGHEST_SIZE,
            0
        );
    }
}

// ------------------------------------------------------------------------------------------------
// Updates all score UI components each frame
// ------------------------------------------------------------------------------------------------
void Score::UpdateIcons()
{
    UpdateCurrentScore();
    UpdateHighestScore();
}
