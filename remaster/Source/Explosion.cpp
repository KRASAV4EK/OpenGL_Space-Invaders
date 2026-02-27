#include "../Headers/Explosion.h"
#include "../Headers/GameSettings.h"
#include "../lib/leetlib.h"

// ------------------------------------------------------------------------------------------------
// Public accessors
// ------------------------------------------------------------------------------------------------
bool Explosion::GetIsExploding() const { return isExploding; }
void Explosion::SetIsExploding(bool value) { isExploding = value; }

// ------------------------------------------------------------------------------------------------
// Starts an explosion animation at the specified position.
// Resets animation state and marks the explosion as active.
// ------------------------------------------------------------------------------------------------
void Explosion::SetupExplosion(float posX, float posY, float size)
{
    explosionX = posX;
    explosionY = posY;
    explosionSize = size;
    isExploding = true;
}

// ------------------------------------------------------------------------------------------------
// Renders one frame of the explosion animation and advances the frame index.
// If the animation finishes, the state resets for the next trigger.
// ------------------------------------------------------------------------------------------------
void Explosion::UpdateExplosion()
{
    // Stop the animation when we've reached the last valid frame
    if (!isExploding || explosionIndex >= Settings.EXPLOSION_SIZE_SPRITES)
    {
        explosionIndex = 0;
        isExploding = false;
        return;
    }

    // Draw one explosion frame and move to the next
    DrawSprite(
        Settings.spritesBoom[explosionIndex++],
        explosionX,
        explosionY + Settings.EXPLOSION_OFFSET_Y, // vertical offset for visual alignment
        explosionSize,
        explosionSize,
        0 // no rotation
    );
}
