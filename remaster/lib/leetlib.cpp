#pragma warning(disable:4995)
#include "leetlib.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <strsafe.h>
#include <math.h>
#include <map>
#include <direct.h>
#include <malloc.h>

#include "fmod/api/inc/fmod.h"
#pragma comment(lib,"lib/fmod/api/lib/fmodvc.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#pragma warning(disable:4244)

//-----------------------------------------------------------------------------
// Global Direct3D state
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Direct3D interface factory
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Primary rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Shared dynamic vertex buffer
bool fullscreen;

// Vertex structure used for sprite rendering
struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // Screen-space position (transformed)
    DWORD color;        // ARGB tint
    float u, v;         // Texture coordinates
};

// Declares the format layout for CUSTOMVERTEX
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


//-----------------------------------------------------------------------------
// Initializes the Direct3D device and presentation parameters.
// Returns S_OK when successful.
//-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)
{
    // Create core Direct3D object
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
        return E_FAIL;

    // Configure device parameters
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = !fullscreen;
    d3dpp.SwapEffect = fullscreen ? D3DSWAPEFFECT_FLIP : D3DSWAPEFFECT_DISCARD;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // V-sync
    d3dpp.BackBufferFormat = fullscreen ? D3DFMT_A8R8G8B8 : D3DFMT_UNKNOWN;
    d3dpp.BackBufferWidth = 800;
    d3dpp.BackBufferHeight = 600;
    d3dpp.FullScreen_RefreshRateInHz = fullscreen ? 60 : 0;

    // Create the Direct3D rendering device
    if (FAILED(g_pD3D->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &g_pd3dDevice)))
    {
        return E_FAIL;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Allocates a dynamic vertex buffer for rendering quads.
// All sprite drawing uses DrawPrimitiveUP instead, but this buffer is kept
// for potential future use or legacy code paths.
//-----------------------------------------------------------------------------
HRESULT InitVB()
{
    if (FAILED(g_pd3dDevice->CreateVertexBuffer(
        4 * sizeof(CUSTOMVERTEX),
        D3DUSAGE_DYNAMIC,
        D3DFVF_CUSTOMVERTEX,
        D3DPOOL_DEFAULT,
        &g_pVB,
        NULL)))
    {
        return E_FAIL;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Frees all Direct3D objects.
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if (g_pVB)        g_pVB->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();
    if (g_pD3D)       g_pD3D->Release();
}


//-----------------------------------------------------------------------------
// Win32 message handler — manages keyboard, mouse, focus, and quit messages.
//-----------------------------------------------------------------------------
bool g_keydown[256]; // Current key states
int  g_keyhit[256];  // Edge-trigger counters (keys pressed this frame)
int  g_mb;           // Mouse button bitmask

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        // Mouse button handlers — maintain internal state & capture
    case WM_LBUTTONDOWN:
        SetCapture(hWnd);
        g_mb |= 1;
        g_keydown[VK_LBUTTON] = true;
        g_keyhit[VK_LBUTTON]++;
        break;

    case WM_RBUTTONDOWN:
        SetCapture(hWnd);
        g_mb |= 2;
        g_keydown[VK_RBUTTON] = true;
        g_keyhit[VK_RBUTTON]++;
        break;

    case WM_MBUTTONDOWN:
        SetCapture(hWnd);
        g_mb |= 4;
        g_keydown[VK_MBUTTON] = true;
        g_keyhit[VK_MBUTTON]++;
        break;

    case WM_LBUTTONUP:
        ReleaseCapture();
        g_mb &= ~1;
        g_keydown[VK_LBUTTON] = false;
        break;

    case WM_RBUTTONUP:
        ReleaseCapture();
        g_mb &= ~2;
        g_keydown[VK_RBUTTON] = false;
        break;

    case WM_MBUTTONUP:
        ReleaseCapture();
        g_mb &= ~4;
        g_keydown[VK_MBUTTON] = false;
        break;

        // Keyboard input
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        g_keydown[wParam & 255] = true;
        g_keyhit[wParam & 255]++;
        return 0;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        g_keydown[wParam & 255] = false;
        break;

        // Window closed
    case WM_DESTROY:
        Cleanup();
        PostQuitMessage(0);
        return 0;

        // App deactivated — clear all keys to avoid stuck inputs
    case WM_ACTIVATEAPP:
        if (!wParam)
            memset(g_keydown, 0, sizeof(g_keydown));
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LARGE_INTEGER starttime;
LARGE_INTEGER freq;
extern HWND hWnd;
HWND hWnd;


//-----------------------------------------------------------------------------
// Application entry point — initializes subsystems and enters the game loop.
//-----------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR cmd, INT)
{
    int id = MessageBox(NULL,"Start in fullscreen mode?","Fullscreen mode",MB_YESNOCANCEL);
    if (id == IDCANCEL) return 0;
    fullscreen = (id == IDYES);
    
    // Register window class
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        MsgProc,
        0L, 0L,
        GetModuleHandle(NULL),
        NULL, NULL, NULL, NULL,
        "SpaceInvaders",
        NULL
    };
    RegisterClassEx(&wc);

    // Window size setup
    RECT r = { 0, 0, 800, 600 };
    int style = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
    style |= WS_VISIBLE;
    AdjustWindowRect(&r, style, false);

    // Create application window
    hWnd = CreateWindow(
        "SpaceInvaders",
        "Space Invaders",
        style,
        0, 0,
        r.right - r.left,
        r.bottom - r.top,
        GetDesktopWindow(),
        NULL,
        wc.hInstance,
        NULL);

    // Initialize FMOD audio
    FSOUND_Init(44100, 42, 0);

    QueryPerformanceCounter(&starttime);
    QueryPerformanceFrequency(&freq);

    // Initialize graphics stack
    if (SUCCEEDED(InitD3D(hWnd)))
    {
        if (SUCCEEDED(InitVB()))
        {
            SetCursor(LoadCursor(NULL, IDC_ARROW));

            ShowWindow(hWnd, SW_SHOWDEFAULT);
            UpdateWindow(hWnd);

            // Enter game logic
            Game();
        }
    }

    UnregisterClass("SpaceInvaders", wc.hInstance);
    return 0;
}


//-----------------------------------------------------------------------------
// Processes Windows messages and prepares the scene for rendering.
// Clears the frame, sets blending, disables depth, and begins a draw scene.
// Returns true when the user has requested to quit.
//-----------------------------------------------------------------------------
bool WantQuit(DWORD clearcol)
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            return true;
    }

    // Clear frame
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, clearcol, 1.0f, 0);

    // Basic render state setup
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);

    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);

    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

    g_pd3dDevice->BeginScene();

    return false;
}


//-----------------------------------------------------------------------------
// Returns the time in milliseconds since program start.
//-----------------------------------------------------------------------------
int GetTimeInMS()
{
    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);
    return int((current.QuadPart - starttime.QuadPart) * 1000 / freq.QuadPart);
}


//-----------------------------------------------------------------------------
// Present the backbuffer contents to the display
//-----------------------------------------------------------------------------
void FlipBuffers()
{
    static int lastflip = 0;
    if (lastflip == 0)
        lastflip = GetTimeInMS();

    g_pd3dDevice->EndScene();

    lastflip = GetTimeInMS();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    Sleep(0);

    memset(g_keyhit, 0, sizeof(g_keyhit));

    SetCursor(LoadCursor(NULL, IDC_ARROW));
}


//-----------------------------------------------------------------------------
// Input helpers
//-----------------------------------------------------------------------------
void GetMousePos(float& x, float& y)
{
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(hWnd, &p);
    x = float(p.x);
    y = float(p.y);
}

void ResetAllKeys()
{
    for (int i = 0; i < 256; i++)
        g_keydown[i] = false;
}

bool IsKeyDown(int key)
{
    return g_keydown[key & 255];
}


//-----------------------------------------------------------------------------
// Sprite rendering helpers
//-----------------------------------------------------------------------------
void* LoadSprite(const char* fname)
{
    IDirect3DTexture9* tex = NULL;
    D3DXCreateTextureFromFile(g_pd3dDevice, fname, &tex);
    return tex;
}

void SetCurrentTexture(void* tex)
{
    g_pd3dDevice->SetTexture(0, (IDirect3DTexture9*)tex);
}

void DrawSprite(void* sprite, float xcentre, float ycentre, float xsize, float ysize, float angle, DWORD col)
{
    SetCurrentTexture(sprite);

    float c = cosf(angle);
    float s = sinf(angle);

#define ROTATE(xx,yy) xcentre + (xx)*c + (yy)*s, ycentre + (yy)*c - (xx)*s

    CUSTOMVERTEX v[] = {
        { ROTATE(-xsize, -ysize), 0.5f, 1.0f, col, 0, 0 },
        { ROTATE(xsize, -ysize), 0.5f, 1.0f, col, 1, 0 },
        { ROTATE(-xsize,  ysize), 0.5f, 1.0f, col, 0, 1 },
        { ROTATE(xsize,  ysize), 0.5f, 1.0f, col, 1, 1 },
    };

    g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(CUSTOMVERTEX));
}


//-----------------------------------------------------------------------------
// Audio: music + SFX wrappers around FMOD
//-----------------------------------------------------------------------------
FSOUND_STREAM* music;

int PlayMusic(const char* fname, float volume)
{
    if (music)
        StopMusic();

    music = FSOUND_Stream_Open(fname, FSOUND_LOOP_NORMAL, 0, 0);
    int chan = FSOUND_Stream_Play(FSOUND_FREE, music);

    volume = max(0.0f, min(volume, 1.0f));
    FSOUND_SetVolume(chan, int(volume * 255));

    return chan;
}

void StopMusic()
{
    if (music)
        FSOUND_Stream_Close(music);

    music = NULL;
}

void* LoadSnd(const char* fname, bool looped)
{
    int flags = looped ? FSOUND_LOOP_NORMAL : 0;
    return FSOUND_Sample_Load(FSOUND_FREE, fname, flags, 0, 0);
}

int PlaySnd(void* sound, float volume)
{
    if (!sound)
        return -1;

    volume = max(0.0f, min(volume, 1.0f));

    int chan = FSOUND_PlaySound(FSOUND_FREE, (FSOUND_SAMPLE*)sound);
    FSOUND_SetVolume(chan, int(volume * 255));

    return chan;
}

void StopSnd(int handle)
{
    if (handle <= 0) return;
    FSOUND_StopSound(handle);
}

void ChangeVolume(int handle, float volume)
{
    if (handle <= 0) return;

    volume = max(0.0f, min(volume, 1.0f));
    FSOUND_SetVolume(handle, int(volume * 255));
}
