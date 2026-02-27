#pragma once 

#include "Object.h"
#include "Bullet.h"
#include "Explosion.h"
#include <vector>

// ------------------------------------------------------------------------------------------------
// User (Player)
//
// Represents the player ship. Handles:
//   - Movement & tilt animation
//   - Bullet management (bullet pool)
//   - Life/health system
//   - Player ship explosion animation
//
// Inherits from:
//   - Object    - shared position, size, speed, Update()/Destroy() contract
//   - Explosion - explosion animation when the player is hit
//
// Notes:
//   - Explosion is inherited privately so only the player manages its own explosion state.
//   - The player owns a fixed-size pool of bullets reused across the game.
// ------------------------------------------------------------------------------------------------
class User : public Object, private Explosion
{
public:
    // Returns a reference to one bullet from the player's bullet pool
    Bullet& GetBulletWithIndex(int i);

    // Initializes the player (position, stats, bullets, etc.)
    void Setup();

    // Main per-frame logic for the player
    void Update() override;

    // Called when the player is destroyed by collision
    void Destroy() override;

private:
    int lifeCount = 1;              // Number of remaining lives
    std::vector<Bullet> bullets;    // Bullet pool for the player
    float tilt = 0.0f;              // Current tilt angle for visual wobble

    // Player update sub-systems
    void UpdatePosition();          // Movement + tilt animation
    void UpdateFire();              // Handle firing input and cooldown
    void UpdateBullets();           // Update all player bullets
    void UpdateLifeIcon() const;    // Draw life indicators (UI hearts/ships)
};

// Global instance representing the player
extern User Player;
