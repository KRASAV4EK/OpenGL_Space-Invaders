#include "../Headers/Bullet.h"
#include "../Headers/GameSettings.h"
#include "../lib/leetlib.h"

// ------------------------------------------------------------------------------------------------
// Initializes bullet parameters based on whether it belongs to the player or an enemy.
// Sets size, speed direction, and angle.
// Bullet starts as inactive and with explosion disabled.
// ------------------------------------------------------------------------------------------------
void Bullet::Setup(bool fromUser)
{
    isAlive = false;          // Bullet is inactive until Fire() is called
    SetIsExploding(false);    // Reset explosion animation

    size = Settings.BULLET_SIZE;
    sizeMultiplier = Settings.BULLET_SIZE_MULTIPLIER;

    // Configure direction and sprite angle
    if (isFromUser = fromUser)
    {
        bulletAngle = Settings.BULLET_ANGLE;
        speed = -Settings.BULLET_SPEED;   // Player bullets move upward
    }
    else
    {
        bulletAngle = -Settings.BULLET_ANGLE;
        speed = Settings.BULLET_SPEED;    // Enemy bullets move downward
    }
}

// ------------------------------------------------------------------------------------------------
// Activates bullet at specific coordinates.
// Bullet becomes "alive" and will begin updating next frame.
// ------------------------------------------------------------------------------------------------
void Bullet::Fire(float x, float y)
{
    posX = x;
    posY = y;
    isAlive = true;
}

// ------------------------------------------------------------------------------------------------
// Per-frame update:
//   - If out of bounds - deactivate
//   - If inactive - run explosion animation (enemy bullets only)
//   - Otherwise move and render sprite
// ------------------------------------------------------------------------------------------------
void Bullet::Update()
{
    // If bullet leaves screen vertically, mark as dead
    if (!IsInVerticalBounds())
        isAlive = false;

    // If bullet is dead, only enemy bullets show an explosion animation
    if (!isAlive)
    {
        if (!isFromUser)
            UpdateExplosion();
        return;
    }

    // Move bullet
    posY += speed;

    // Draw bullet sprite
    DrawSprite(
        Settings.spriteBullet,
        posX,
        posY,
        size,
        size,
        bulletAngle
    );
}

// ------------------------------------------------------------------------------------------------
// Called when bullet collides with an object.
// Enemy bullets trigger a small explosion on death.
// ------------------------------------------------------------------------------------------------
void Bullet::Destroy()
{
    isAlive = false;

    // Only enemy bullets explode visually
    if (!isFromUser)
        SetupExplosion(posX, posY, Settings.EXPLOSION_SIZE_BULLET);
}
