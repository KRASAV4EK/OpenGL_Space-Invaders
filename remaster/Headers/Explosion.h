#pragma once

// ------------------------------------------------------------------------------------------------
// Explosion
//
// Utility class used to play a frame-based explosion animation.
// The owning object (e.g., Enemy, Player, Bullet) triggers SetupExplosion()
// and then calls UpdateExplosion() every frame until the animation ends.
//
// This class does not manage its own lifetime; it simply renders
// one explosion animation sequence when requested.
// ------------------------------------------------------------------------------------------------
class Explosion
{
public:
    // Returns true while the explosion animation is currently active.
    bool GetIsExploding() const;

    // Manually override explosion state (rarely needed, mainly for cleanup).
    void SetIsExploding(bool value);

protected:
    // Initializes a new explosion at a given world position and size.
    // Resets the animation index and enables rendering.
    void SetupExplosion(float posX, float posY, float size);

    // Advances the explosion animation frame-by-frame.
    // Automatically stops once the animation reaches the last sprite.
    void UpdateExplosion();

private:
    // Explosion world position (center point).
    float explosionX = 0.0f;
    float explosionY = 0.0f;

    // Current animation frame index.
    int explosionIndex = 0;

    // True if the explosion animation is currently playing.
    bool isExploding = false;

    // Base rendered size of the explosion sprite.
    float explosionSize = 1.0f;
};
