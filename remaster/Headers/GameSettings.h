#pragma once

#include <iostream>

// ------------------------------------------------------------------------------------------------
// GameSettings
//
// Central configuration container for all static and runtime gameplay parameters.
// This includes window size, player movement tuning, enemy layout, bullet values,
// explosion animation settings, UI coordinates, sprite pointers, etc.
//
// The class acts as a globally accessible read-only/mutable settings bank.
// Runtime-updated values (e.g., tick counters, current enemy count) are also stored here.
//
// Sprite loading and some simple background/title rendering helpers are implemented
// inside this class as well.
//
// A global instance named `Settings` is declared at the bottom of this file.
// ------------------------------------------------------------------------------------------------
class GameSettings
{
public:
    // ---- Window dimensions ----
    const int   WINDOW_WIDTH = 800;
    const int   WINDOW_HEIGHT = 600;

    // ---- Background rendering ----
    const float BACKGROUND_START_X = 0.0f;
    const float BACKGROUND_START_Y = 0.0f;
    const float BACKGROUND_SCALE_X = 1.5f;
    const float BACKGROUND_SCALE_Y = 1.5f;
    const float BACKGROUND_WIDTH = WINDOW_WIDTH * BACKGROUND_SCALE_X;
    const float BACKGROUND_HEIGHT = WINDOW_HEIGHT * BACKGROUND_SCALE_Y;

    // ---- Player settings ----
    const int   PLAYER_LIFES_AMOUNT = 3;
    const float PLAYER_START_X = 400.0f;
    const float PLAYER_START_Y = 550.0f;
    const float PLAYER_SPEED = 5.0f;
    const float PLAYER_SIZE = 50.0f;
    const float PLAYER_SIZE_MULTIPLIER = 1.0f;
    const float PLAYER_TILT_MAX = 0.25f;       // Maximum tilt applied visually when moving
    const float PLAYER_SMOOTH_SPEED = 0.15f;   // Smoothing factor for interpolation

    // UI: player lives display
    const float PLAYER_LIFE_START_X = 710.0f;
    const float PLAYER_LIFE_START_X_OFFSET = 30.0f;
    const float PLAYER_LIFE_START_Y = 30.0f;
    const float PLAYER_LIFE_SIZE = 15.0f;

    // ---- Enemy settings ----
    const int   ENEMIES_AMOUNT_MAX = 50;
    int   ENEMIES_AMOUNT_CURRENT = ENEMIES_AMOUNT_MAX; // Live count updated during gameplay
    const int   ENEMIES_ROWS = 5;
    const int   ENEMIES_COLS = 10;
    const float ENEMY_SPEED = 1.0f;
    const float ENEMY_SPEEDUP = 0.05f;  // Speed increase after each kill
    const float ENEMY_SIZE = 10.0f;
    const int   ENEMY_SHOOT_COEFFICIENT = 1000;             // Higher = less frequent shooting
    const int   ENEMY_SHOOT_IMPROVEMENT_PER_ENEMY = 20;     // Shooting frequency improves over time
    const float ENEMY_SHOOT_IMPROVEMENT_PER_STAGE = 0.8f;   // Lowering the shoot coefficient before new stage
    const float ENEMY_SIZE_MULTIPLIER = 0.8f;
    const float ENEMY_START_X = 60.0f;
    const float ENEMY_START_Y = 70.0f;
    const float ENEMY_START_X_OFFSET = 60.0f;
    const float ENEMY_START_Y_OFFSET = 60.0f;
    const float ENEMY_BOUNCE_OFFSET_Y = 1.0f;
    const float ENEMY_DEAD_OFFSET_Y = 0.1f; // Small vertical drop when an enemy dies

    // ---- Bullet settings ----
    const int   BULLET_AMOUNT = 10;     // Maximum concurrent bullets
    const int   BULLET_COOLDOWN = 20;   // Frames between shots
    const float BULLET_SPEED = 4.0f;
    const float BULLET_SIZE = 10.0f;
    const float BULLET_ANGLE = 1.6f;
    const float BULLET_SIZE_MULTIPLIER = 0.5f;

    // ---- UI: title text layout ----
    const float TITLE_START_X = 150;
    const float TITLE_START_X_OFFSET = 40;
    const float TITLE_START_Y = 30;
    const float TITLE_SIZE = 20;
    const float TITLE_SIZE_SPRITES = 14;

    // ---- UI: score layout ----
    const float SCORE_CURRENT_X = 20;
    const float SCORE_CURRENT_X_OFFSET = 25;
    const float SCORE_CURRENT_Y = 30;
    const float SCORE_CURRENT_SIZE = 20;

    const float SCORE_HIGHEST_X = 20;
    const float SCORE_HIGHEST_X_OFFSET = 15;
    const float SCORE_HIGHEST_Y = 70;
    const float SCORE_HIGHEST_SIZE = 10;

    // ---- Explosion settings ----
    const int   EXPLOSION_SIZE_SPRITES = 33; // Number of animation frames
    const float EXPLOSION_SIZE_SHIP = 100.0f;
    const float EXPLOSION_SIZE_BULLET = 50.0f;
    const float EXPLOSION_OFFSET_Y = 12.0f;

    // ---- Tint colors (ARGB) ----
    const unsigned int TINT_COLOR_NORMAL = 0xffffffff;
    const unsigned int TINT_COLOR_RED = 0xffff0000;
    const unsigned int TINT_COLOR_GREEN = 0xff00ff00;
    const unsigned int TINT_COLOR_BLUE = 0xff0000ff;
    const unsigned int TINT_COLOR_ALPHA = 0x80ffffff; // 50% transparency

    // ---- Error codes ----
    const int ERROR_DIVISION_BY_0 = 1;

    // ---- Runtime state ----
    int tick = 0;          // Global tick counter, used for animations
    bool gameNeedsQuit = false; // Main loop exit flag

    // ---- Sprite pointers (object textures) ----
    void* spriteEnemy = nullptr;
    void* spritePlayer = nullptr;
    void* spriteBullet = nullptr;
    void* spriteBlackHole = nullptr;

    // UI sprites (letters for the title)
    void* spritesTitle[14] = { nullptr };

    // Number sprites for score display
    void* spritesNumbers[10] = { nullptr };

    // Explosion animation frames
    void* spritesBoom[33] = { nullptr };

    // ------------------------------------------------------------------------------------------------
    // Template utility function: basic division-by-zero safety guard
    // Logs the offending variable and exits the program.
    // ------------------------------------------------------------------------------------------------
    template<typename T>
    void DivisionByZeroCheck(T object)
    {
        if (object == 0)
        {
            std::cerr << "Error: Division by zero! " << typedef(object) << " = 0" << std::endl;
            exit(Settings.ERROR_DIVISION_BY_0);
        }
    }

    // Loads all sprites used by the game
    void SetupSprites();

    // Renders animated background
    void UpdateBackground() const;

    // Renders animated title text
    void UpdateTitle() const;
};

// Global GameSettings instance
extern GameSettings Settings;
