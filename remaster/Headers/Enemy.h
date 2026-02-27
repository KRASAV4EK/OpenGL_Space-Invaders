#pragma once

#include "Object.h"
#include "Bullet.h"
#include "Explosion.h"
#include "GameSettings.h"

// ------------------------------------------------------------------------------------------------
// Enemy
//
// Represents a single invader in the enemy formation. Handles:
//   - Horizontal marching + periodic vertical descent
//   - Per-enemy bullet management
//   - Row-based tint coloring
//   - Scoring and stage progression
//   - Explosion effects
//
// Behaviors are coordinated across the entire enemy grid through several static
// values shared between all Enemy instances:
//
//   - speed                - marching speed of the whole formation
//   - moveRight/moveDown   - direction flags for formation movement
//   - verticalOffset       - cumulative downward movement
//   - deadOffset           - extra vertical drop after enemies die
//   - shootCoefficient     - difficulty-based firing probability
//
// Each Enemy owns exactly one Bullet that can be reused.
// Explosion is inherited privately, so only the enemy manages its own explosion state.
// ------------------------------------------------------------------------------------------------
class Enemy : public Object, private Explosion
{
public:
    Bullet& GetBullet();
    bool GetAbleToFire() const;
    void SetAbleToFire(bool ableToFire);

    // Initializes enemy based on its index in the enemy grid
    void Setup(int i);

    // Main per-frame update
    void Update() override;

    // Handles enemy destruction, scoring, speed scaling, and stage progression
    void Destroy() override;

    // Called when all enemies are destroyed; resets formation for next stage
    void LoadNewStage();

    // Resets all shared enemy-state variables (speed, direction, coefficients)
    static void Reset();

private:
    int indexArray = 0;              // Index in the enemy array (0–49 by default)
    int rewardPoints = 1;            // Score value for killing this enemy
    Bullet bullet;                   // Each enemy controls its own bullet
    bool isAbleToFire = false;       // Only bottom-row enemies start able to fire
    unsigned int tintColor = Settings.TINT_COLOR_NORMAL;

    // Shared behavior/state for all enemies in the formation
    static int shootCoefficient, lastShootCoefficient;
    static float minPosX, minPosY;
    static float maxPosX, maxPosY;
    static bool moveRight, moveDown;
    static float speed;
    static float verticalOffset;     // How far down the formation has moved
    static float deadOffset;         // Extra drop applied after each death (difficulty scaling)

    // Subsystems
    void UpdateBullet();
    void UpdatePosition();
    void UpdateFire();
    void SetTint();

    // Reset helper functions
    static void ResetOffset();
    static void ResetShootCoefficient();
};

// Global array of all enemies
extern Enemy Enemies[50];
