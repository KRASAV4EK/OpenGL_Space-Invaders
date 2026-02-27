#include "../Headers/User.h"
#include "../Headers/GameSettings.h"
#include "../lib/leetlib.h"

// ------------------------------------------------------------------------------------------------
// Player movement logic.
// Handles:
//   - Horizontal movement
//   - Boundary clamping
//   - Tilt animation based on movement direction
//   - Subtle wobble animation for extra visual polish
// ------------------------------------------------------------------------------------------------
void User::UpdatePosition()
{
    if (!isAlive) return;

    float oldPosX = posX;

    // Input checks (arrow keys or WASD)
    bool left = IsKeyDown(VK_LEFT) || IsKeyDown('A');
    bool right = IsKeyDown(VK_RIGHT) || IsKeyDown('D');

    if (left)  posX -= speed;
    if (right) posX += speed;

    // Prevent leaving screen bounds
    if (!IsInHorizontalBounds())
        posX = oldPosX;

    // Target tilt depending on movement direction
    float targetTilt = 0.0f;
    if (left)  targetTilt += Settings.PLAYER_TILT_MAX;
    if (right) targetTilt -= Settings.PLAYER_TILT_MAX;

    // Smooth interpolation of tilt
    tilt += (targetTilt - tilt) * Settings.PLAYER_SMOOTH_SPEED;

    // Extra subtle wobble effect (breathing-like motion)
    float wobble = sin(Settings.tick * 0.1f) * 0.05f;

    DrawSprite(Settings.spritePlayer, posX, posY, size, size, tilt + wobble);
}

// ------------------------------------------------------------------------------------------------
// Player shooting logic.
// Manages:
//   - Cooldown timer between shots
//   - Cycling through a pool of reusable bullets
//   - Firing from the player's current position
// ------------------------------------------------------------------------------------------------
void User::UpdateFire()
{
    if (!isAlive) return;

    static int bulletIndex = 0; // Current bullet in pool
    static int coolDown = 0;    // Time before next allowed shot

    // Decrease cooldown timer
    if (coolDown) --coolDown;

    // Fire on spacebar if cooldown expired
    if (IsKeyDown(VK_SPACE) && coolDown == 0)
    {
        // Prevent overwriting a bullet still active on screen
        if (bullets[bulletIndex].GetIsAlive())
            return;

        bullets[bulletIndex].Fire(posX, posY);

        // Move to next bullet in pool
        bulletIndex = (bulletIndex + 1) % Settings.BULLET_AMOUNT;
        coolDown = Settings.BULLET_COOLDOWN;
    }
}

// ------------------------------------------------------------------------------------------------
// Updates all bullets owned by the player.
//
// If no enemies remain, all bullets are forcibly disabled until the next round.
// Otherwise each bullet performs its own Update().
// ------------------------------------------------------------------------------------------------
void User::UpdateBullets()
{
    for (auto& bullet : bullets)
    {
        if (Settings.ENEMIES_AMOUNT_CURRENT <= 0)
            bullet.SetIsAlive(false);
        else
            bullet.Update();
    }

    // Reset enemy count when reaching zero (round reset behavior)
    if (Settings.ENEMIES_AMOUNT_CURRENT <= 0)
        Settings.ENEMIES_AMOUNT_CURRENT = Settings.ENEMIES_AMOUNT_MAX;
}

// Returns one bullet from the player's bullet pool.
Bullet& User::GetBulletWithIndex(int i)
{
    return bullets[i];
}

// ------------------------------------------------------------------------------------------------
// Initializes all player values at the start of the game or after a reset.
// Includes:
//   - Position
//   - Speed and size parameters
//   - Lives
//   - Bullet pool initialization
//   - Explosion state reset
// ------------------------------------------------------------------------------------------------
void User::Setup()
{
    lifeCount = Settings.PLAYER_LIFES_AMOUNT;

    // Player spawn position
    startX = posX = Settings.PLAYER_START_X;
    startY = posY = Settings.PLAYER_START_Y;

    speed = Settings.PLAYER_SPEED;
    size = Settings.PLAYER_SIZE;
    sizeMultiplier = Settings.PLAYER_SIZE_MULTIPLIER;

    isAlive = true;
    SetIsExploding(false);

    // Initialize bullet pool
    bullets.resize(Settings.BULLET_AMOUNT);
    for (auto& bullet : bullets)
        bullet.Setup(true); // true - bullet belongs to player
};

// ------------------------------------------------------------------------------------------------
// Draws extra life icons (small ships) in the UI.
// ------------------------------------------------------------------------------------------------
void User::UpdateLifeIcon() const
{
    for (int i = 0; i < lifeCount; i++)
    {
        DrawSprite(
            Settings.spritePlayer,
            Settings.PLAYER_LIFE_START_X + Settings.PLAYER_LIFE_START_X_OFFSET * i,
            Settings.PLAYER_LIFE_START_Y,
            Settings.PLAYER_LIFE_SIZE,
            Settings.PLAYER_LIFE_SIZE,
            0
        );
    }
}

// ------------------------------------------------------------------------------------------------
// Player main update function.
// Executes in order:
//   1. Movement
//   2. Shooting
//   3. Bullet updates
//   4. UI life icons
//   5. Explosion animation (if currently exploding)
// ------------------------------------------------------------------------------------------------
void User::Update()
{
    UpdatePosition();
    UpdateFire();
    UpdateBullets();

    UpdateLifeIcon();
    UpdateExplosion();
};

// ------------------------------------------------------------------------------------------------
// Handles player death.
//   - Spawns explosion at player's position
//   - Decrements life count
//   - Marks player dead if out of lives
// ------------------------------------------------------------------------------------------------
void User::Destroy()
{
    SetupExplosion(posX, posY, Settings.EXPLOSION_SIZE_SHIP);

    if (--lifeCount <= 0)
        isAlive = false;
}
