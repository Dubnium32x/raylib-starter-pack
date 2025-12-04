// =============================================================
// Global Definitions Header
// =============================================================
// Global variables, constants, and instances used across the project
#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Visual settings
#define VIRTUAL_SCREEN_WIDTH 1280
#define VIRTUAL_SCREEN_HEIGHT 720

// Global constants 
#define VERSION "0.1.0"
#define GAME_TITLE "Raylib Starter Pack"

#define TILE_SIZE 16

// Screen settings
extern int screenWidth;
extern int screenHeight;
extern int windowSize;
extern bool isFullscreen;
extern bool isVSync;
extern bool isDebugMode;

extern RenderTexture2D g_virtualScreen;

// Fonts 
extern Font g_fontFamily[3];

extern Font geetRegular;
extern Font malvidesRegular;
extern Font neu5landNormal;

// Function to get mouse position in virtual screen coordinates
Vector2 GetMousePositionVirtual();

#endif // GLOBALS_H