#pragma once

// ------------------------------------------------------------------------------------------------
// Base class for all in-game entities.
//
// Provides common state such as position, size, speed, and "alive" flag.
// Derived classes must implement Update() and Destroy().
//
// This class intentionally contains only minimal shared functionality.
// More complex game objects extend or override behavior in child classes.
// ------------------------------------------------------------------------------------------------
class Object
{
public:
    // Called once per frame; must be implemented by derived objects
    virtual void Update() = 0;

    // Called when the object is being hit
    virtual void Destroy() = 0;

    // ---- Public getters ----
    bool GetIsAlive() const;
    float GetPosX() const;
    float GetPosY() const;
    float GetSize() const;
    float GetSizeMultiplier() const;

    // ---- Public setters ----
    void SetIsAlive(bool value);
    void SetPosX(float value);
    void SetPosY(float value);

protected:
    // Initial spawn coordinates
    float startX = 0.0f;
    float startY = 0.0f;

    // Current world position
    float posX = 0.0f;
    float posY = 0.0f;

    // Base movement speed 
    float speed = 0.0f;

    // Base visual size of the object 
    float size = 1.0f;

    // Additional optional scaling factor (used for collision)
    float sizeMultiplier = 1.0f;

    // Indicates whether the object is active in the game world
    bool isAlive = false;

    // Boundary checks against the active window region
    bool IsInHorizontalBounds() const;
    bool IsInVerticalBounds() const;

    // Combined bounds check
    bool IsInBounds() const;

private:
    // (Currently unused — reserved for future internal data)
};
