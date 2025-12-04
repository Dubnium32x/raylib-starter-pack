// =============================================================
// Screen Settings header
// =============================================================
// Defines screen settings and configurations
#ifndef SCREEN_SETTINGS_H
#define SCREEN_SETTINGS_H
#include "raylib.h"
#include "../util/globals.h"

// Screen setting aliases - these reference the global variables
#define GAME_SCREEN_WIDTH screenWidth
#define GAME_SCREEN_HEIGHT screenHeight
#define GAME_WINDOW_SIZE windowSize
#define GAME_FULLSCREEN isFullscreen
#define GAME_VSYNC isVSync
#define GAME_DEBUG_MODE isDebugMode

// Screen setting initialization state
extern bool screenSettingsInitialized;

// Window size options
typedef enum {
    ONE_X = 1,
    TWO_X = 2,
    THREE_X = 3,
    FOUR_X = 4
} WindowSize;

// Window type options
typedef enum {
    WINDOW_TYPE_FULLSCREEN,
    WINDOW_TYPE_WINDOWED,
    WINDOW_TYPE_BORDERLESS
} WindowType;

// Function prototypes
void InitScreenSettings();
void ApplyScreenSettings(int width, int height, int windowSize, bool fullscreen, bool vsync, bool debugMode);
void ToggleGameFullscreen();
void SetVSync(bool vsync);
void SetDebugMode(bool debugMode);
void PrestoSetWindowSize(int size);
void UpdateScreenSettings(int width, int height, int windowSize, bool fullscreen, bool vsync, bool debugMode);
void GetScreenSettings(int* width, int* height, int* windowSize, bool* fullscreen, bool* vsync, bool* debugMode);
void UnloadScreenSettings();
#endif // SCREEN_SETTINGS_H