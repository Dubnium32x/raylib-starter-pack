// =============================================================
// 2D Handler System Implementation
// =============================================================
// Core 2D game framework handler implementation

#include "handler2d.h"
#include "../world/screen_manager.h"
#include <stdio.h>
#include <string.h>

// External screen manager
extern ScreenManager g_screenManager;

// Global handler instance
Handler2D g_handler2D = {0};

// Initialize the 2D handler
bool InitHandler2D(void) {
    printf("Initializing 2D Handler...\n");
    
    // Initialize game state
    memset(&g_handler2D, 0, sizeof(Handler2D));
    g_handler2D.gameState.isRunning = true;
    g_handler2D.gameState.isPaused = false;
    g_handler2D.gameState.isDebugMode = false;
    g_handler2D.gameState.deltaTime = 0.0f;
    g_handler2D.gameState.currentFPS = 0;
    g_handler2D.gameState.gameTime = 0.0f;
    
    // Initialize screen management
    g_handler2D.currentScreen = SCREEN_STATE_INIT;
    g_handler2D.nextScreen = SCREEN_STATE_INIT;
    g_handler2D.screenTransition = false;
    g_handler2D.transitionTimer = 0.0f;
    g_handler2D.transitionDuration = 1.0f;
    
    // Initialize timing
    g_handler2D.lastUpdateTime = GetTime();
    g_handler2D.accumulator = 0.0f;
    *(float*)&g_handler2D.fixedTimestep = 1.0f/60.0f; // 60 FPS target
    
    // Initialize debug settings
    g_handler2D.showDebugInfo = false;
    g_handler2D.showFPS = false;
    
    // Mark as initialized
    g_handler2D.initialized = true;
    
    printf("✓ 2D Handler initialized successfully\n");
    return true;
}

// Main update function
void UpdateHandler2D(void) {
    if (!g_handler2D.initialized || !g_handler2D.gameState.isRunning) {
        return;
    }
    
    // Calculate delta time
    float currentTime = GetTime();
    float frameTime = currentTime - g_handler2D.lastUpdateTime;
    g_handler2D.lastUpdateTime = currentTime;
    
    // Update game time
    if (!g_handler2D.gameState.isPaused) {
        g_handler2D.gameState.gameTime += frameTime;
    }
    
    // Store delta time
    g_handler2D.gameState.deltaTime = frameTime;
    g_handler2D.gameState.currentFPS = GetFPS();
    
    // Handle screen transitions
    if (g_handler2D.screenTransition) {
        g_handler2D.transitionTimer += frameTime;
        if (g_handler2D.transitionTimer >= g_handler2D.transitionDuration) {
            g_handler2D.currentScreen = g_handler2D.nextScreen;
            g_handler2D.screenTransition = false;
            g_handler2D.transitionTimer = 0.0f;
        }
    }
    
    // Process input
    _Handler2D_ProcessInput();
    
    // Update game logic if not paused
    if (!g_handler2D.gameState.isPaused) {
        _Handler2D_UpdateGameLogic(frameTime);
    }
    
    // Update screen manager if available
    if (g_screenManager.initialized) {
        UpdateScreenManager(&g_screenManager, frameTime);
    }
}

// Main render function
void RenderHandler2D(void) {
    if (!g_handler2D.initialized) {
        return;
    }
    
    BeginDrawing();
    
    // Clear screen
    ClearBackground(BLACK);
    
    // Begin virtual screen rendering
    BeginTextureMode(g_virtualScreen);
    ClearBackground(BLACK);
    
    // Render game content
    _Handler2D_RenderGame();
    
    // Render screen manager content
    if (g_screenManager.initialized) {
        DrawScreenManager(&g_screenManager);
    }
    
    // Render debug info if enabled
    if (g_handler2D.showDebugInfo) {
        _Handler2D_RenderDebugInfo();
    }
    
    EndTextureMode();
    
    // Render virtual screen to actual screen
    Rectangle source = { 0.0f, 0.0f, (float)g_virtualScreen.texture.width, (float)-g_virtualScreen.texture.height };
    Rectangle dest = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
    Vector2 origin = { 0.0f, 0.0f };
    
    DrawTexturePro(g_virtualScreen.texture, source, dest, origin, 0.0f, WHITE);
    
    // Render FPS if enabled
    if (g_handler2D.showFPS) {
        DrawText(TextFormat("FPS: %d", g_handler2D.gameState.currentFPS), 10, 10, 20, GREEN);
    }
    
    EndDrawing();
}

// Shutdown the handler
void ShutdownHandler2D(void) {
    printf("Shutting down 2D Handler...\n");
    
    g_handler2D.gameState.isRunning = false;
    g_handler2D.initialized = false;
    
    printf("✓ 2D Handler shut down\n");
}

// Screen management functions
void SetScreen(ScreenType newScreen) {
    if (newScreen != g_handler2D.currentScreen) {
        printf("Switching screen from %d to %d\n", g_handler2D.currentScreen, newScreen);
        g_handler2D.currentScreen = newScreen;
        
        // Also update screen manager if available
        if (g_screenManager.initialized) {
            SetCurrentScreen(&g_screenManager, newScreen);
        }
    }
}

void RequestScreenTransition(ScreenType newScreen, float duration) {
    g_handler2D.nextScreen = newScreen;
    g_handler2D.transitionDuration = duration;
    g_handler2D.transitionTimer = 0.0f;
    g_handler2D.screenTransition = true;
}

bool IsScreenTransitioning(void) {
    return g_handler2D.screenTransition;
}

// Game state management
void PauseGame(void) {
    g_handler2D.gameState.isPaused = true;
}

void ResumeGame(void) {
    g_handler2D.gameState.isPaused = false;
}

void TogglePause(void) {
    g_handler2D.gameState.isPaused = !g_handler2D.gameState.isPaused;
}

bool IsGamePaused(void) {
    return g_handler2D.gameState.isPaused;
}

// Debug functions
void ToggleDebugMode(void) {
    g_handler2D.gameState.isDebugMode = !g_handler2D.gameState.isDebugMode;
    g_handler2D.showDebugInfo = g_handler2D.gameState.isDebugMode;
}

void ToggleDebugInfo(void) {
    g_handler2D.showDebugInfo = !g_handler2D.showDebugInfo;
}

void ToggleFPSDisplay(void) {
    g_handler2D.showFPS = !g_handler2D.showFPS;
}

// Utility functions
float GetDeltaTime(void) {
    return g_handler2D.gameState.deltaTime;
}

float GetGameTime(void) {
    return g_handler2D.gameState.gameTime;
}

int GetCurrentFPS(void) {
    return g_handler2D.gameState.currentFPS;
}

// Internal functions
void _Handler2D_ProcessInput(void) {
    // Handle global debug keys
    if (IsKeyPressed(KEY_F1)) {
        ToggleDebugMode();
    }
    
    if (IsKeyPressed(KEY_F2)) {
        ToggleFPSDisplay();
    }
    
    if (IsKeyPressed(KEY_F3)) {
        ToggleDebugInfo();
    }
    
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_PAUSE)) {
        TogglePause();
    }
    
    // Check for exit
    if (IsKeyPressed(KEY_ESCAPE) && g_handler2D.currentScreen == SCREEN_STATE_TITLE) {
        g_handler2D.gameState.isRunning = false;
    }
    
    // Global debug keys are handled directly by raylib
}

void _Handler2D_UpdateGameLogic(float deltaTime) {
    // Update game systems here
    // This is where you would update entities, physics, audio, etc.
    
    // For now, just basic screen-specific logic
    switch (g_handler2D.currentScreen) {
        case SCREEN_STATE_INIT:
            // Auto-transition to title after a short delay
            if (g_handler2D.gameState.gameTime > 2.0f) {
                SetScreen(SCREEN_STATE_TITLE);
            }
            break;
            
        case SCREEN_STATE_TITLE:
            // Handle title screen input
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                SetScreen(SCREEN_STATE_DEBUG1);
            }
            if (IsKeyPressed(KEY_O)) {
                SetScreen(SCREEN_STATE_OPTIONS);
            }
            break;
            
        default:
            break;
    }
}

void _Handler2D_RenderGame(void) {
    // Basic screen rendering - will be expanded by screen manager
    switch (g_handler2D.currentScreen) {
        case SCREEN_STATE_INIT:
            break;
            
        case SCREEN_STATE_TITLE:
            TextureFilter(TEXTURE_FILTER_BILINEAR);
            DrawTextEx(g_fontFamily[1], "RAYLIB STARTER PACK", 
                       (Vector2){VIRTUAL_SCREEN_WIDTH/2 - 200, VIRTUAL_SCREEN_HEIGHT/2 - 40}, 
                       40, 2, WHITE);
            DrawTextEx(g_fontFamily[2], "Press ENTER or SPACE to Start", 
                       (Vector2){VIRTUAL_SCREEN_WIDTH/2 - 150, VIRTUAL_SCREEN_HEIGHT/2 + 20}, 
                       20, 1, LIGHTGRAY);
            DrawTextEx(g_fontFamily[2], "Press O for Options", 
                       (Vector2){VIRTUAL_SCREEN_WIDTH/2 - 80, VIRTUAL_SCREEN_HEIGHT/2 + 50}, 
                       20, 1, LIGHTGRAY);   
            DrawTextEx(g_fontFamily[2], "Press ESC to Exit", 
                       (Vector2){VIRTUAL_SCREEN_WIDTH/2 - 70, VIRTUAL_SCREEN_HEIGHT/2 + 80}, 
                       20, 1, LIGHTGRAY);
            break;
            
        default:
            break;
    }
}

void _Handler2D_RenderDebugInfo(void) {
    if (!g_handler2D.showDebugInfo) return;
    
    int y = 50;
    int lineHeight = 15;
    
    DrawText("=== DEBUG INFO ===", 10, y, 12, YELLOW);
    y += lineHeight * 2;
    
    DrawText(TextFormat("Screen: %d", g_handler2D.currentScreen), 10, y, 12, WHITE);
    y += lineHeight;
    
    DrawText(TextFormat("Game Time: %.2f", g_handler2D.gameState.gameTime), 10, y, 12, WHITE);
    y += lineHeight;
    
    DrawText(TextFormat("Delta Time: %.4f", g_handler2D.gameState.deltaTime), 10, y, 12, WHITE);
    y += lineHeight;
    
    DrawText(TextFormat("Paused: %s", g_handler2D.gameState.isPaused ? "YES" : "NO"), 10, y, 12, WHITE);
    y += lineHeight;
    
    DrawText(TextFormat("Transition: %s", g_handler2D.screenTransition ? "YES" : "NO"), 10, y, 12, WHITE);
    y += lineHeight;
    
    if (g_handler2D.screenTransition) {
        DrawText(TextFormat("Transition Timer: %.2f/%.2f", 
                g_handler2D.transitionTimer, g_handler2D.transitionDuration), 10, y, 12, WHITE);
        y += lineHeight;
    }
    
    y += lineHeight;
    DrawText("Controls:", 10, y, 12, YELLOW);
    y += lineHeight;
    DrawText("F1 - Toggle Debug Mode", 10, y, 10, LIGHTGRAY);
    y += lineHeight;
    DrawText("F2 - Toggle FPS Display", 10, y, 10, LIGHTGRAY);
    y += lineHeight;
    DrawText("F3 - Toggle Debug Info", 10, y, 10, LIGHTGRAY);
    y += lineHeight;
    DrawText("P - Toggle Pause", 10, y, 10, LIGHTGRAY);
}