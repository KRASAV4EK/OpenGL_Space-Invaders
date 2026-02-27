#pragma once

#include <windows.h>

typedef unsigned long       DWORD;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// System layer
// The game always renders at a fixed resolution of 800x600.
// The expected main loop calls WantQuit() at the start of each frame, and FlipBuffers() at the end.
// WantQuit() handles Windows messages and clears the screen; it returns true when the user requests exit.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool WantQuit(DWORD clearcolor = 0);

// Presents the backbuffer to the display and enforces a 60 Hz frame rate cap.
// Also resets per-frame input states (e.g., key hits).
void FlipBuffers();

// Main game entry point — implemented by the user/game logic.
void Game();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Retrieves the current mouse position in client coordinates.
// Top-left corner = (0,0), bottom-right = (800,600).
void GetMousePos(float& x, float& y);

// Clears the internal key states; useful when the app loses and regains focus.
void ResetAllKeys();

// Returns true while a key is held down.
// Accepts Windows VK_* codes (e.g., VK_LEFT) or ASCII characters ('A', '0').
// Supports mouse buttons via VK_LBUTTON, VK_RBUTTON.
bool IsKeyDown(int key);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rendering — Sprite API
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Loads an image file into a Direct3D texture.
// Returns an opaque pointer to the texture; pass it to DrawSprite().
void* LoadSprite(const char* fname);

// Draws a textured quad at a given center position, size, rotation, and ARGB tint.
// Rotation is in radians. Tint allows color modulation (0xffffffff = no tint).
void DrawSprite(
    void* sprite,
    float xcentre,
    float ycentre,
    float xsize,
    float ysize,
    float rotate_angle_radians = 0,
    DWORD tint_col_argb = 0xffffffff
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Audio
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Starts music playback from a file, looping automatically.
// Returns a channel handle used for later volume adjustments via ChangeVolume().
int PlayMusic(const char* fname, float volume = 1);

// Stops currently playing music immediately.
void StopMusic();

// Loads a sound effect sample; can be configured to loop indefinitely.
void* LoadSnd(const char* fname, bool loop = false);

// Plays a sound effect once. Returns a channel handle for StopSnd() or ChangeVolume().
int PlaySnd(void* sound, float volume = 1);

// Stops an active sound effect on a specific channel.
void StopSnd(int handle);

// Changes the playback volume of a sound effect channel.
void ChangeVolume(int handle, float newvolume = 1);
