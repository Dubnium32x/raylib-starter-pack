// =============================================================
// Init Screen Implementation
// =============================================================
// Handles game initialization and startup sequence

#include "init_screen.h"
#include "../util/globals.h"
#include "../world/screen_manager.h"
#include "../2d/handler2d.h"
#include <stdio.h>

// Init screen state
static float initTimer = 0.0f;
static float fadeAlpha = 255.0f;
static bool initialized = false;

void InitScreen_Init(void) {
    printf("Initializing Init Screen...\n");
    
    initTimer = 0.0f;
    fadeAlpha = 255.0f;
    initialized = true;
    
    printf("✓ Init Screen ready\n");
}

void InitScreen_Update(float deltaTime) {
    if (!initialized) return;
    
    initTimer += deltaTime;
    
    // Fade in effect
    if (fadeAlpha > 0.0f) {
        fadeAlpha -= 150.0f * deltaTime;
        if (fadeAlpha < 0.0f) fadeAlpha = 0.0f;
    }
    
    // Auto-transition to title screen after 3 seconds
    if (initTimer > 3.0f) {
        SetScreen(SCREEN_STATE_TITLE);
    }
    
    // Skip with any key
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        SetScreen(SCREEN_STATE_TITLE);
    }
}

void InitScreen_Draw(void) {
    if (!initialized) return;
    
    // Background
    ClearBackground(BLACK);
    
    // Center text
    int centerX = VIRTUAL_SCREEN_WIDTH / 2;
    int centerY = VIRTUAL_SCREEN_HEIGHT / 2;
    
    // Title
    const char* title = "RAYLIB STARTER PACK";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, centerX - titleWidth/2, centerY - 60, 40, WHITE);
    
    // Version
    const char* version = "Version " VERSION;
    int versionWidth = MeasureText(version, 20);
    DrawText(version, centerX - versionWidth/2, centerY - 10, 20, LIGHTGRAY);
    
    // Loading indicator
    const char* loading = "Loading...";
    int loadingWidth = MeasureText(loading, 16);
    DrawText(loading, centerX - loadingWidth/2, centerY + 30, 16, GRAY);
    
    // Simple loading animation
    int dotCount = ((int)(initTimer * 2) % 4);
    for (int i = 0; i < dotCount; i++) {
        DrawCircle(centerX + loadingWidth/2 + 20 + i*10, centerY + 38, 3, WHITE);
    }
    
    // Skip hint
    if (initTimer > 1.0f) {
        const char* skip = "Press any key to skip";
        int skipWidth = MeasureText(skip, 12);
        DrawText(skip, centerX - skipWidth/2, centerY + 80, 12, DARKGRAY);
    }
    
    // Fade overlay
    if (fadeAlpha > 0.0f) {
        DrawRectangle(0, 0, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT, 
                     (Color){0, 0, 0, (unsigned char)fadeAlpha});
    }
}

void InitScreen_Unload(void) {
    printf("Unloading Init Screen...\n");
    initialized = false;
}