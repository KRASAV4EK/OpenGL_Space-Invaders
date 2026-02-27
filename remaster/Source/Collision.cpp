#include "../Headers/Collision.h"
#include "../Headers/GameSettings.h"
#include "../Headers/User.h"
#include "../Headers/Enemy.h"
#include <cmath>

// ------------------------------------------------------------------------------------------------
// Collide
//
// Circle–circle collision check between two Object instances.
// ------------------------------------------------------------------------------------------------
static bool Collide(Object& first, Object& second)
{
    float firstX = first.GetPosX();
    float firstY = first.GetPosY();
    float firstRadius = first.GetSize() * first.GetSizeMultiplier();

    float secondX = second.GetPosX();
    float secondY = second.GetPosY();
    float secondRadius = second.GetSize() * second.GetSizeMultiplier();

    float dx = firstX - secondX;
    float dy = firstY - secondY;
    float r = firstRadius + secondRadius;

    // Basic circle-overlap test
    if (dx * dx + dy * dy <= r * r)
    {
        // Collision occurred
        first.Destroy();
        second.Destroy();
        return true;
    }
    return false;
}

// ------------------------------------------------------------------------------------------------
// CheckCollisions
//
// Handles all collision combinations performed each frame:
//
//   (1) Player bullets vs Enemies
//       - When a player bullet hits an enemy:
//             - That enemy is destroyed
//             - If the enemy was able to fire, the next enemy in the same column
//               (above it) becomes the new shooter
//
//   (2) Player vs Enemies
//       - If any enemy reaches the player - both are destroyed
//
//   (3) Player vs Enemy bullets
//       - Standard collision; enemy bullets can kill the player
//
//   (4) Player bullets vs Enemy bullets
//       - Bullets destroy each other when colliding
//
// This function relies on:
//      - Global Player object
//      - Global Enemies array
//      - Settings for array sizes
//
// The function short-circuits inner loops once a collision occurs to reduce redundant checks.
// ------------------------------------------------------------------------------------------------
void CheckCollisions()
{
    // --------------------------------------------------------------------------------------------
    // (1) Player bullets vs Enemies
    // --------------------------------------------------------------------------------------------
    for (int b = 0; b < Settings.BULLET_AMOUNT; ++b)
    {
        Bullet& bullet = Player.GetBulletWithIndex(b);
        if (!bullet.GetIsAlive()) continue;

        for (int e = 0; e < Settings.ENEMIES_AMOUNT_MAX; ++e)
        {
            Enemy& enemy = Enemies[e];
            if (!enemy.GetIsAlive()) continue;

            if (Collide(bullet, enemy))
            {
                // After an enemy dies, enable fire for the next enemy behind it in the same column
                if (enemy.GetAbleToFire() && e - Settings.ENEMIES_COLS >= 0)
                {
                    int behindEnemyIndex = e - Settings.ENEMIES_COLS;
                    while (behindEnemyIndex >= 0)
                    {
                        if (Enemies[behindEnemyIndex].GetIsAlive())
                        {
                            Enemies[behindEnemyIndex].SetAbleToFire(true);
                            break;
                        }
                        behindEnemyIndex -= Settings.ENEMIES_COLS;
                    }
                }
                break;
            }
        }
    }

    // --------------------------------------------------------------------------------------------
    // (2) Player vs Enemies
    // --------------------------------------------------------------------------------------------
    for (int e = 0; e < Settings.ENEMIES_AMOUNT_MAX; ++e)
    {
        if (!Enemies[e].GetIsAlive()) continue;

        if (Collide(Player, Enemies[e]))
            break; // Player already destroyed - stop checking
    }

    // --------------------------------------------------------------------------------------------
    // (3) Player vs Enemy bullets
    // --------------------------------------------------------------------------------------------
    for (int e = 0; e < Settings.ENEMIES_AMOUNT_MAX; ++e)
    {
        Bullet& enemyBullet = Enemies[e].GetBullet();
        if (!enemyBullet.GetIsAlive()) continue;

        if (Collide(Player, enemyBullet))
            break;
    }

    // --------------------------------------------------------------------------------------------
    // (4) Player bullets vs Enemy bullets
    // --------------------------------------------------------------------------------------------
    for (int e = 0; e < Settings.ENEMIES_AMOUNT_MAX; ++e)
    {
        Bullet& enemyBullet = Enemies[e].GetBullet();
        if (!enemyBullet.GetIsAlive()) continue;

        for (int b = 0; b < Settings.BULLET_AMOUNT; ++b)
        {
            Bullet& playerBullet = Player.GetBulletWithIndex(b);
            if (!playerBullet.GetIsAlive()) continue;

            if (Collide(playerBullet, enemyBullet))
                break; // Stop checking more player bullets for this enemy bullet
        }
    }
}
