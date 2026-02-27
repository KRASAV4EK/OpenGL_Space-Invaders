#include <math.h>

#include "../Headers/GameSettings.h"
#include "../Headers/User.h"
#include "../Headers/Enemy.h"
#include "../Headers/Collision.h"
#include "../Headers/Score.h"

#include "../lib/leetlib.h"

// ------------------------------------------------------------------------------------------------
// Global instances
// These objects are intentionally global because the game architecture is structured
// around a classic retro-style main loop with globally coordinated state.
// ------------------------------------------------------------------------------------------------
GameSettings Settings;     // Global settings holder (window, constants, sprites, etc.)
User Player;               // The player-controlled ship
Enemy Enemies[50];         // Fixed-size enemy formation (5 rows × 10 columns)

// ------------------------------------------------------------------------------------------------
// Update()
//
// Performs one full frame update for all game systems except input polling and collision.
// Called once per frame inside the main loop.
// ------------------------------------------------------------------------------------------------
static void Update()
{
    Settings.UpdateBackground();

    Player.Update();

    for (int i = 0; i < Settings.ENEMIES_AMOUNT_MAX; i++)
    {
        Enemies[i].Update();
    }

    Settings.UpdateTitle();

    Score::UpdateIcons();
}

// ------------------------------------------------------------------------------------------------
// MainCycle()
//
// Runs a full game session until:
//   - Player dies, OR
//   - ESC key is pressed, OR
//   - Window close is requested.
//
// Each frame:
//   1. Increase frame tick (used for animation)
//   2. Check collisions
//   3. Update game objects
//   4. Render (FlipBuffers)
//
// When the player dies, the game prompts for restart.
// ------------------------------------------------------------------------------------------------
static void MainCycle()
{
    while (Player.GetIsAlive() &&
        !(Settings.gameNeedsQuit = (IsKeyDown(VK_ESCAPE) || WantQuit())))
    {
        Settings.tick++;          // Global animation frame counter

        CheckCollisions();        // Handle all collision groups

        Update();                 // Update all game objects

        FlipBuffers();            // Render everything to the screen
    }

    // Save highscore after every session
    Score::Save();

    // If user exited with ESC or window close - skip restart prompt
    if (!Settings.gameNeedsQuit)
    {
        ResetAllKeys();   // Avoid input bleed-through into MessageBox
        int id = MessageBoxA(NULL, "Restart?", "Game Over", MB_YESNO);

        if (id == IDYES)
            Settings.gameNeedsQuit = false;
        else
            Settings.gameNeedsQuit = true;

        // Reset formation-wide enemy state
        Enemy::Reset();
    }
}

// ------------------------------------------------------------------------------------------------
// SetupObjects()
//
// Initializes all runtime objects for a new game session:
//   - Loads highest score from disk
//   - Initializes player state
//   - Initializes entire enemy formation
// ------------------------------------------------------------------------------------------------
static void SetupObjects()
{
    Score::Load();

    Player.Setup();

    for (int i = 0; i < Settings.ENEMIES_AMOUNT_MAX; i++)
    {
        Enemies[i].Setup(i);
    }
}

// ------------------------------------------------------------------------------------------------
// Game()
//
// Entry point for the entire gameplay:
//   1. Load sprites
//   2. Loop over game sessions until the user chooses to quit
//   3. Show final highscore message
//
// This function is called by the framework in leetlib (WinMain - Game()).
// ------------------------------------------------------------------------------------------------
void Game()
{
    Settings.SetupSprites();

    do
    {
        SetupObjects();
        MainCycle();

    } while (!Settings.gameNeedsQuit);

    // Final goodbye message after quitting
    std::string message =
        "Highscore: " + std::to_string(Score::GetHighest()) + ". Thanks for playing!";

    MessageBoxA(NULL, message.c_str(), "Quit", MB_OK);
}
