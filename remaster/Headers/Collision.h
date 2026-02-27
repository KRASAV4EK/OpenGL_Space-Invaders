#pragma once

#include "../Headers/Object.h"

// ------------------------------------------------------------------------------------------------
// Collide
//
// Performs a simple circle–circle collision test between two Object instances.
// Uses the object’s position plus its size * sizeMultiplier to compute radius.
// If a collision occurs:
//     - both objects have Destroy() called on them
//     - returns true
// Otherwise returns false.
//
// This function is defined as static in the translation unit (Collision.cpp) and
// declared here for use within this module.
// ------------------------------------------------------------------------------------------------
static bool Collide(Object& first, Object& second);

// ------------------------------------------------------------------------------------------------
// CheckCollisions
//
// Centralized collision dispatcher used once per game frame.
// Handles all major collision groups:
//
//   1. Player bullets     vs Enemies
//   2. Player             vs Enemies
//   3. Player             vs Enemy bullets
//   4. Player bullets     vs Enemy bullets
//
// Includes additional logic for enabling the next enemy in a column to fire
// after the one in front is destroyed.
//
// This function depends on global Player and Enemies containers defined elsewhere.
// ------------------------------------------------------------------------------------------------
void CheckCollisions();
