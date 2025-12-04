// =============================================================
// 2D Handler System Header
// =============================================================
// Core 2D game framework handler that manages game state,
// rendering, input, and system coordination
#ifndef HANDLER2D_H
#define HANDLER2D_H

#include "raylib.h"
#include "../util/globals.h"
#include "../world/screen_state.h"

// Game state structure
typedef struct {
    bool isRunning;
    bool isPaused;
    bool isDebugMode;
    float deltaTime;
    int currentFPS;
    float gameTime;
} GameState;

// 2D Handler structure
typedef struct {
    GameState gameState;
    ScreenType currentScreen;
    ScreenType nextScreen;
    bool screenTransition;
    float transitionTimer;
    float transitionDuration;
    
    // Timing
    float lastUpdateTime;
    float accumulator;
    const float fixedTimestep;
    
    // Debug info
    bool showDebugInfo;
    bool showFPS;
    
    // Initialization flags
    bool initialized;
} Handler2D;

// Global handler instance
extern Handler2D g_handler2D;

// Core functions
bool InitHandler2D(void);
void UpdateHandler2D(void);
void RenderHandler2D(void);
void ShutdownHandler2D(void);

// Screen management
void SetScreen(ScreenType newScreen);
void RequestScreenTransition(ScreenType newScreen, float duration);
bool IsScreenTransitioning(void);

// Game state management
void PauseGame(void);
void ResumeGame(void);
void TogglePause(void);
bool IsGamePaused(void);

// Debug functions
void ToggleDebugMode(void);
void ToggleDebugInfo(void);
void ToggleFPSDisplay(void);

// Utility functions
float GetDeltaTime(void);
float GetGameTime(void);
int GetCurrentFPS(void);

// Internal functions (for framework use)
void _Handler2D_ProcessInput(void);
void _Handler2D_UpdateGameLogic(float deltaTime);
void _Handler2D_RenderGame(void);
void _Handler2D_RenderDebugInfo(void);

#endif // HANDLER2D_H
