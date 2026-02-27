#pragma once

#include "Object.h"
#include "Score.h"
#include "Explosion.h"

// ------------------------------------------------------------------------------------------------
// Bullet
//
// Represents a single projectile fired either by the player or an enemy.
// A Bullet:
//
//   - moves vertically (up for player, down for enemies)
//   - deactivates itself when leaving screen bounds
//   - triggers a small explosion on enemy-fired bullets when destroyed
//
// The class inherits from:
//   - Object    - base position/size/update structure
//   - Explosion - small explosion animation for enemy bullets only
//
// Bullets are lightweight, reused objects managed by the owning system
// (player firing, enemy firing, or object pool).
// ------------------------------------------------------------------------------------------------
class Bullet : public Object, public Explosion
{
public:
    // Initializes bullet defaults (size, direction, angle) depending on user/enemy origin
    void Setup(bool fromUser);

    // Activates the bullet at the specified position
    void Fire(float x, float y);

    // Per-frame update: movement, bounds checking, rendering
    void Update() override;

    // Called when bullet collides with something or needs to be terminated
    void Destroy() override;

private:
    // True if fired by the player, false if fired by an enemy
    bool isFromUser = true;

    // Visual rotation of the bullet sprite
    float bulletAngle = 0.0f;
};
