#include "../Headers/Object.h"
#include "../Headers/GameSettings.h"

// ------------------------------------------------------------------------------------------------
// Getters — provide read-only access to core object properties
// ------------------------------------------------------------------------------------------------
bool Object::GetIsAlive() const { return isAlive; }
float Object::GetPosX() const { return posX; }
float Object::GetPosY() const { return posY; }
float Object::GetSize() const { return size; }
float Object::GetSizeMultiplier() const { return sizeMultiplier; }

// ------------------------------------------------------------------------------------------------
// Setters — update object runtime values
// ------------------------------------------------------------------------------------------------
void Object::SetIsAlive(bool value) { isAlive = value; }
void Object::SetPosX(float value) { posX = value; }
void Object::SetPosY(float value) { posY = value; }

// ------------------------------------------------------------------------------------------------
// Boundary checks
//
// These helpers ensure that objects stay inside the visible gameplay area.
// They use window dimensions from GameSettings.
// The checks use "size" so the object's edges stay within bounds, not the center.
// ------------------------------------------------------------------------------------------------
bool Object::IsInHorizontalBounds() const
{
    return (posX >= size && posX <= Settings.WINDOW_WIDTH - size);
}

bool Object::IsInVerticalBounds() const
{
    return (posY >= size && posY <= Settings.WINDOW_HEIGHT - size);
}

bool Object::IsInBounds() const
{
    return IsInHorizontalBounds() && IsInVerticalBounds();
}
