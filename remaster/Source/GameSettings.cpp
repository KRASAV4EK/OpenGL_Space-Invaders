#include "../Headers/GameSettings.h"
#include "../lib/leetlib.h"
#include <chrono>
#include <string>

// ------------------------------------------------------------------------------------------------
// Loads all textures required by the game: player, enemies, UI numbers, letters, explosions, etc.
// This method is called once during initialization.
// ------------------------------------------------------------------------------------------------
void GameSettings::SetupSprites()
{
    // Core game object sprites
    spriteEnemy     = LoadSprite("gfx/Little Invader.png");
    spritePlayer    = LoadSprite("gfx/Big Invader.png");
    spriteBullet    = LoadSprite("gfx/Bullet.png");
    spriteBlackHole = LoadSprite("gfx/Background/Black Hole.png");

    // Number sprites 0–9 for score rendering
    for (int i = 0; i < 10; i++)
    {
        spritesNumbers[i] =
            LoadSprite(("gfx/Numbers/num" + std::to_string(i) + ".png").c_str());
    }

    // Title lettering sprites (manually assigned)
    spritesTitle[0]  = LoadSprite("gfx/Letters/slet.png");
    spritesTitle[1]  = LoadSprite("gfx/Letters/plet.png");
    spritesTitle[2]  = LoadSprite("gfx/Letters/alet.png");
    spritesTitle[3]  = LoadSprite("gfx/Letters/clet.png");
    spritesTitle[4]  = LoadSprite("gfx/Letters/elet.png");
    spritesTitle[5]  = 0; // Empty spacing
    spritesTitle[6]  = LoadSprite("gfx/Letters/ilet.png");
    spritesTitle[7]  = LoadSprite("gfx/Letters/nlet.png");
    spritesTitle[8]  = LoadSprite("gfx/Letters/vlet.png");
    spritesTitle[9]  = LoadSprite("gfx/Letters/alet.png");
    spritesTitle[10] = LoadSprite("gfx/Letters/dlet.png");
    spritesTitle[11] = LoadSprite("gfx/Letters/elet.png");
    spritesTitle[12] = LoadSprite("gfx/Letters/rlet.png");
    spritesTitle[13] = LoadSprite("gfx/Letters/slet.png");

    // Explosion animation frames
    for (int i = 0; i < EXPLOSION_SIZE_SPRITES; i++)
    {
        spritesBoom[i] =
            LoadSprite(("gfx/Explosion/img_" + std::to_string(i) + ".png").c_str());
    }
}

// ------------------------------------------------------------------------------------------------
// Renders the animated background. The black hole slowly rotates based on the global tick counter.
// ------------------------------------------------------------------------------------------------
void GameSettings::UpdateBackground() const
{
    DrawSprite(
        spriteBlackHole,
        BACKGROUND_START_X,
        BACKGROUND_START_Y,
        BACKGROUND_WIDTH,
        BACKGROUND_HEIGHT,
        sin(tick * 0.001f) // slow rotational animation
    );
}

// ------------------------------------------------------------------------------------------------
// Draws the animated game title at the top of the screen.
// Letter 5 is intentionally skipped (visual spacing).
// Slight sinusoidal rotation gives a retro “floating” effect.
// ------------------------------------------------------------------------------------------------
void GameSettings::UpdateTitle() const
{
    for (size_t i = 0; i < TITLE_SIZE_SPRITES; i++)
    {
        // Skip the spacing slot
        if (i == 5)
            continue;

        DrawSprite(
            spritesTitle[i],
            TITLE_START_X + TITLE_START_X_OFFSET * i,
            TITLE_START_Y,
            TITLE_SIZE,
            TITLE_SIZE,
            sin(tick * 0.01f) * 0.1f // subtle animation
        );
    }
}
