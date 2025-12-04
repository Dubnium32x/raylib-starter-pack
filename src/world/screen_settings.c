// =============================================================
// Screen Settings source
// =============================================================
// Implements functions to manage screen settings in the game
#include "screen_settings.h"

// Initialize the screen settings
void InitScreenSettings() {
    // Initialize screen settings with default values
    windowSize = ONE_X;
    screenWidth = VIRTUAL_SCREEN_WIDTH * windowSize;
    screenHeight = VIRTUAL_SCREEN_HEIGHT * windowSize;
    isFullscreen = false;
    isVSync = true;
    isDebugMode = false;

    // Apply initial settings
    ApplyScreenSettings(screenWidth, screenHeight, windowSize, isFullscreen, isVSync, isDebugMode);
}

// Apply the given screen settings
void ApplyScreenSettings(int width, int height, int winSize, bool fullscreen, bool vsync, bool debugMode) {
    // Set screen dimensions
    screenWidth = width;
    screenHeight = height;
    windowSize = winSize;
    isFullscreen = fullscreen;
    isVSync = vsync;
    isDebugMode = debugMode;

    // Calculate actual window size
    int actualWidth = screenWidth * (windowSize + 1);
    int actualHeight = screenHeight * (windowSize + 1);

    // Set window mode
    if (isFullscreen) {
        SetWindowState(FLAG_FULLSCREEN_MODE);
        SetWindowSize(actualWidth, actualHeight);
    } else {
        ClearWindowState(FLAG_FULLSCREEN_MODE);
        SetWindowSize(actualWidth, actualHeight);
        SetWindowPosition((GetMonitorWidth(0) - actualWidth) / 2, (GetMonitorHeight(0) - actualHeight) / 2);
    }

    // Set VSync
    SetConfigFlags(isVSync ? FLAG_VSYNC_HINT : 0);

    // Set debug mode (if applicable)
    if (isDebugMode) {
        // Enable debug features here
        printf("Debug mode enabled\n");
    } else {
        // Disable debug features here
        printf("Debug mode disabled\n");
    }

    // Apply changes
    SetWindowTitle(GAME_TITLE " - " VERSION);
}

// Toggle fullscreen mode
void ToggleGameFullscreen() {
    if (IsWindowFullscreen()) {
        ToggleBorderlessWindowed();
        SetWindowSize(screenWidth, screenHeight);
    } else {
        ToggleBorderlessWindowed();
    }
}

// Set VSync option
void SetVSync(bool vsync) {
    isVSync = vsync;
    ApplyScreenSettings(screenWidth, screenHeight, windowSize, isFullscreen, isVSync, isDebugMode);
}

// Set debug mode option
void SetDebugMode(bool debugMode) {
    isDebugMode = debugMode;
    ApplyScreenSettings(screenWidth, screenHeight, windowSize, isFullscreen, isVSync, isDebugMode);
}

// Set window size option
void PrestoSetWindowSize(int size) {
    windowSize = size;
    ApplyScreenSettings(screenWidth, screenHeight, windowSize, isFullscreen, isVSync, isDebugMode);
}

// Update screen settings
void UpdateScreenSettings(int width, int height, int winSize, bool fullscreen, bool vsync, bool debugMode) {
    ApplyScreenSettings(width, height, winSize, fullscreen, vsync, debugMode);
}

// Get current screen settings
void GetScreenSettings(int* width, int* height, int* winSize, bool* fullscreen, bool* vsync, bool* debugMode) {
    if (width) *width = screenWidth;
    if (height) *height = screenHeight;
    if (winSize) *winSize = windowSize;
    if (fullscreen) *fullscreen = isFullscreen;
    if (vsync) *vsync = isVSync;
    if (debugMode) *debugMode = isDebugMode;
}

// Unload screen settings
void UnloadScreenSettings() {
    // Cleanup if necessary
}