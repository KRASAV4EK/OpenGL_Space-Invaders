#include "../Headers/Enemy.h"
#include "../Headers/Score.h"
#include "../lib/leetlib.h"

// ------------------------------------------------------------------------------------------------
// Static member initialization for shared formation behavior
// These values modulate how the *entire enemy group* behaves.
// ------------------------------------------------------------------------------------------------
int   Enemy::shootCoefficient = Settings.ENEMY_SHOOT_COEFFICIENT;
int   Enemy::lastShootCoefficient = Settings.ENEMY_SHOOT_COEFFICIENT;

float Enemy::minPosX = Settings.ENEMY_SIZE * (Settings.ENEMIES_ROWS - 1);
float Enemy::minPosY = Settings.ENEMY_SIZE * (Settings.ENEMIES_ROWS - 1);
float Enemy::maxPosX = Settings.WINDOW_WIDTH - Settings.ENEMY_SIZE * (Settings.ENEMIES_ROWS - 1);
float Enemy::maxPosY = Settings.WINDOW_HEIGHT - Settings.ENEMY_SIZE * (Settings.ENEMIES_ROWS - 1);

float Enemy::verticalOffset = 0.0f;
float Enemy::deadOffset = 0.0f;

bool  Enemy::moveRight = true;
bool  Enemy::moveDown = true;

float Enemy::speed = Settings.ENEMY_SPEED;

// ------------------------------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------------------------------
Bullet& Enemy::GetBullet() { return bullet; }
bool Enemy::GetAbleToFire() const { return isAbleToFire; }
void Enemy::SetAbleToFire(bool ableToFire) { isAbleToFire = ableToFire; }

// ------------------------------------------------------------------------------------------------
// Assigns enemy color tint based on vertical row in the formation.
// Lower rows become different tint colors for visual variation/difficulty feedback.
// ------------------------------------------------------------------------------------------------
void Enemy::SetTint()
{
    if (indexArray < 0 || indexArray >= Settings.ENEMIES_AMOUNT_MAX)
        return;

    int secondRow = Settings.ENEMIES_COLS * (Settings.ENEMIES_ROWS - 3);
    int thirdRow = Settings.ENEMIES_COLS * (Settings.ENEMIES_ROWS - 2);
    int fourthRow = Settings.ENEMIES_COLS * (Settings.ENEMIES_ROWS - 1);

    if (indexArray < Settings.ENEMIES_COLS)
        tintColor = Settings.TINT_COLOR_NORMAL;
    else if (indexArray < secondRow)
        tintColor = Settings.TINT_COLOR_RED;
    else if (indexArray < thirdRow)
        tintColor = Settings.TINT_COLOR_GREEN;
    else if (indexArray < fourthRow)
        tintColor = Settings.TINT_COLOR_BLUE;
    else
        tintColor = Settings.TINT_COLOR_ALPHA;
}

// ------------------------------------------------------------------------------------------------
// Sets initial enemy parameters based on its index in the formation grid.
// Computes position, size, tint, ability to fire, and initializes the bullet.
// ------------------------------------------------------------------------------------------------
void Enemy::Setup(int i)
{
    indexArray = i;
    SetTint();

    // Score reward based on row (higher rows give more points)
    rewardPoints = Settings.ENEMIES_ROWS - (indexArray / Settings.ENEMIES_COLS);

    // Compute grid-based spawn position
    startX = posX = (indexArray % Settings.ENEMIES_COLS) * Settings.ENEMY_START_X_OFFSET + Settings.ENEMY_START_X;
    startY = posY = (indexArray / Settings.ENEMIES_COLS) * Settings.ENEMY_START_Y_OFFSET + Settings.ENEMY_START_Y;

    // Adjust size slightly depending on row for subtle variation
    Settings.DivisionByZeroCheck(Settings.ENEMIES_COLS);
    size = Settings.ENEMY_SIZE + (i / Settings.ENEMIES_COLS) * (Settings.ENEMIES_ROWS - 1);
    sizeMultiplier = Settings.ENEMY_SIZE_MULTIPLIER;

    // Set X movement boundaries
    minPosX = size;
    maxPosX = Settings.WINDOW_WIDTH - size;

    isAlive = true;

    // Only bottom row enemies can initially fire
    isAbleToFire = (indexArray >= Settings.ENEMIES_AMOUNT_MAX - Settings.ENEMIES_COLS);

    bullet.Setup(false); // false - enemy bullet
}

// ------------------------------------------------------------------------------------------------
// Reset vertical offsets for the formation (used between waves)
// ------------------------------------------------------------------------------------------------
void Enemy::ResetOffset()
{
    verticalOffset = 0.0f;
    deadOffset = 0.0f;
}

// ------------------------------------------------------------------------------------------------
// Restores initial shooting difficulty values
// ------------------------------------------------------------------------------------------------
void Enemy::ResetShootCoefficient()
{
    shootCoefficient = Settings.ENEMY_SHOOT_COEFFICIENT;
    lastShootCoefficient = shootCoefficient;
}

// ------------------------------------------------------------------------------------------------
// Resets *global* enemy state (all shared values).
// Used when restarting or beginning a fresh game.
// ------------------------------------------------------------------------------------------------
void Enemy::Reset()
{
    ResetOffset();
    ResetShootCoefficient();
    moveRight = true;
    speed = Settings.ENEMY_SPEED;
    Settings.ENEMIES_AMOUNT_CURRENT = Settings.ENEMIES_AMOUNT_MAX;
}

// ------------------------------------------------------------------------------------------------
// Called when all enemies have been eliminated.
// Decreases shoot coefficient (increasing difficulty) and reinitializes the stage.
// ------------------------------------------------------------------------------------------------
void Enemy::LoadNewStage()
{
    ResetOffset();

    lastShootCoefficient *= Settings.ENEMY_SHOOT_IMPROVEMENT_PER_STAGE; // Increase firing frequency
    shootCoefficient = lastShootCoefficient;

    // Recreate entire formation
    for (int i = 0; i < Settings.ENEMIES_AMOUNT_MAX; i++)
        Enemies[i].Setup(i);
}

// ------------------------------------------------------------------------------------------------
// Handles enemy death:
//   - Kill enemy, play explosion
//   - Add score reward
//   - Improve enemy firing rate
//   - Increase movement speed
//   - Add vertical drop (classic Space Invaders behavior)
//   - When formation is cleared - load next stage
// ------------------------------------------------------------------------------------------------
void Enemy::Destroy()
{
    isAlive = false;
    SetupExplosion(posX, posY, Settings.EXPLOSION_SIZE_SHIP);

    Score::Add(rewardPoints);

    // Improve firing odds for remaining enemies
    if (shootCoefficient > Settings.ENEMY_SHOOT_IMPROVEMENT_PER_ENEMY)
        shootCoefficient -= Settings.ENEMY_SHOOT_IMPROVEMENT_PER_ENEMY;

    // Speed up remaining enemies
    speed += Settings.ENEMY_SPEEDUP;

    // Lower the formation slightly after a kill
    deadOffset += Settings.ENEMY_DEAD_OFFSET_Y;

    // If all enemies are dead, start a new stage
    if (--Settings.ENEMIES_AMOUNT_CURRENT <= 0)
        LoadNewStage();
}

// ------------------------------------------------------------------------------------------------
// Updates the enemy bullet each frame
// ------------------------------------------------------------------------------------------------
void Enemy::UpdateBullet()
{
    bullet.Update();
}

// ------------------------------------------------------------------------------------------------
// Handles movement of a single enemy as part of the marching formation.
// Movement rules:
//   - Move horizontally until hitting boundary, then
//        - Reverse direction
//        - Move down by verticalOffset
//   - Formation may stop descending once reaching near the bottom
// ------------------------------------------------------------------------------------------------
void Enemy::UpdatePosition()
{
    // Hit left boundary - switch to moving right
    if (posX <= minPosX)
    {
        moveRight = true;
        if (moveDown)
            verticalOffset += Settings.ENEMY_BOUNCE_OFFSET_Y + deadOffset;
    }
    // Hit right boundary - switch to moving left
    else if (posX >= maxPosX)
    {
        moveRight = false;
        if (moveDown)
            verticalOffset += Settings.ENEMY_BOUNCE_OFFSET_Y + deadOffset;
    }

    // Horizontal marching
    if (moveRight) posX += speed;
    else           posX -= speed;

    // Compute new vertical position
    posY = startY + verticalOffset;

    // Stop descending once reaching bottom
    if (posY >= maxPosY)
        moveDown = false;

    DrawSprite(Settings.spriteEnemy, posX, posY, size, size, 0, tintColor);
}

// ------------------------------------------------------------------------------------------------
// Enemy firing logic.
// Enemy fires randomly based on current shooting coefficient.
// Only enemies that:
//   - are alive,
//   - are marked as allowed to fire,
//   - have their bullet available,
//   - are not currently exploding,
//   - AND pass a probability check,
// may shoot.
// ------------------------------------------------------------------------------------------------
void Enemy::UpdateFire()
{
    if (shootCoefficient <= 0)
        shootCoefficient = Settings.ENEMY_SHOOT_IMPROVEMENT_PER_ENEMY;

    if (!isAbleToFire ||
        bullet.GetIsAlive() ||
        bullet.GetIsExploding() ||
        rand() % shootCoefficient != 0)
        return;

    bullet.Fire(posX, posY);
}

// ------------------------------------------------------------------------------------------------
// Per-frame update for this enemy.
// ------------------------------------------------------------------------------------------------
void Enemy::Update()
{
    UpdateBullet();

    if (!isAlive)
    {
        UpdateExplosion();
        return;
    }

    UpdatePosition();
    UpdateFire();
}
