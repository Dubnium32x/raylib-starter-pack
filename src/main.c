// =============================================================
// RAYLIB STARTER PACK
// =============================================================
// Main entry point for the raylib starter framework
// A flexible 2D game development framework built on raylib
// =============================================================

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Framework includes
#include "util/globals.h"
#include "util/asset_manager.h"
#include "2d/handler2d.h"
#include "world/screen_manager.h"
#include "world/screen_state.h"
#include "screen/init_screen.h"
#include "screen/debug_test_1.h"
#include "screen/debug_test_2.h"
#include "screen/debug_test_3.h"

// Global instances
ScreenManager g_screenManager = {0};
static Music g_backgroundMusic = {0};
static Music g_debugMusic = {0};
static float g_musicFadeVolume = 0.3f;
static bool g_musicTransitioning = false;

// Global screen and rendering
RenderTexture2D g_virtualScreen = {0};

// Global fonts
Font g_fontFamily[3] = {0};
Font geetRegular = {0};
Font malvidesRegular = {0};
Font neu5landNormal = {0};

// Screen settings
int screenWidth = 1280;
int screenHeight = 720;
int windowSize = 2;
bool isFullscreen = false;
bool isVSync = true;
bool isDebugMode = false;

// Forward declarations
bool InitializeGame(void);
void UpdateGame(void);
void RenderGame(void);
void ShutdownGame(void);
void RegisterAllScreens(void);

// Music transition functions
void StartDebugMusic(void);
void StartMainMusic(void);

// Main entry point
int main(void) {
    printf("=== RAYLIB STARTER PACK ===\n");
    printf("Version: %s\n", VERSION);
    printf("Initializing framework...\n\n");
    
    // Initialize the game
    if (!InitializeGame()) {
        printf("Failed to initialize game!\n");
        return -1;
    }
    
    printf("Starting main game loop...\n");
    
    // Main game loop
    while (!WindowShouldClose() && g_handler2D.gameState.isRunning) {
        UpdateGame();
        RenderGame();
    }
    
    // Cleanup
    ShutdownGame();
    
    printf("Game shut down successfully.\n");
    return 0;
}

// Initialize all game systems
bool InitializeGame(void) {
    // Initialize raylib window
    printf("Initializing window (%dx%d)...\n", screenWidth, screenHeight);
    InitWindow(screenWidth, screenHeight, GAME_TITLE);
    
    if (isVSync) {
        SetTargetFPS(60);
    }
    
    if (isFullscreen) {
        ToggleFullscreen();
    }
    
    // Initialize audio
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        printf("Warning: Audio device not available\n");
    }
    
    // Create virtual screen for resolution-independent rendering
    g_virtualScreen = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
    if (g_virtualScreen.id == 0) {
        printf("Failed to create virtual screen!\n");
        CloseWindow();
        return false;
    }
    
    // Initialize asset manager
    InitAssetManager();
    
    // Load fonts
    printf("Loading fonts...\n");
    geetRegular = LoadFont("res/font/geet.regular.ttf");
    malvidesRegular = LoadFont("res/font/malvides.regular.otf");
    neu5landNormal = LoadFont("res/font/neu5land.normal.ttf");
    
    // Populate font family array
    g_fontFamily[0] = geetRegular;
    g_fontFamily[1] = malvidesRegular;
    g_fontFamily[2] = neu5landNormal;
    
    printf("✓ Fonts loaded\n");
    
    // Initialize screen manager
    InitScreenManager(&g_screenManager);
    printf("✓ Screen manager initialized\n");
    
    // Initialize 2D handler (core framework)
    if (!InitHandler2D()) {
        printf("Failed to initialize 2D handler!\n");
        UnloadRenderTexture(g_virtualScreen);
        CloseWindow();
        return false;
    }
    
    // Register all screens with the screen manager
    RegisterAllScreens();
    
    // Load and start background music
    g_backgroundMusic = LoadMusicStream("res/audio/music/Rob Gasser, Miss Lina - Rift [NCS Release].mp3");
    if (g_backgroundMusic.stream.buffer != NULL) {
        SetMusicVolume(g_backgroundMusic, 0.3f); // Set to 30% volume
        PlayMusicStream(g_backgroundMusic);
        printf("✓ Background music loaded and started\n");
    } else {
        printf("⚠ Could not load background music\n");
    }
    
    // Load debug music for later use
    g_debugMusic = LoadMusicStream("res/audio/music/Rob Gasser - Ricochet [NCS Release].mp3");
    if (g_debugMusic.stream.buffer != NULL) {
        printf("✓ Debug music loaded\n");
    } else {
        printf("⚠ Could not load debug music\n");
    }
    
    // Set initial screen
    SetCurrentScreen(&g_screenManager, SCREEN_STATE_INIT);
    
    printf("✓ Game initialization complete!\n\n");
    return true;
}

// Register all screens with the screen manager
void RegisterAllScreens(void) {
    printf("Registering screens...\n");
    
    // Register core screens
    RegisterScreen(&g_screenManager, SCREEN_STATE_INIT, 
                   InitScreen_Init, InitScreen_Update, InitScreen_Draw, InitScreen_Unload);
    
    // Register debug/test screens
    RegisterScreen(&g_screenManager, SCREEN_STATE_DEBUG1,
                   DebugTest1_Init, DebugTest1_Update, DebugTest1_Draw, DebugTest1_Unload);
                   
    RegisterScreen(&g_screenManager, SCREEN_STATE_DEBUG2,
                   DebugTest2_Init, DebugTest2_Update, DebugTest2_Draw, DebugTest2_Unload);
                   
    RegisterScreen(&g_screenManager, SCREEN_STATE_DEBUG3,
                   DebugTest3_Init, DebugTest3_Update, DebugTest3_Draw, DebugTest3_Unload);
    
    printf("✓ All screens registered\n");
}

// Update game logic
void UpdateGame(void) {
    // Update music streams
    if (g_backgroundMusic.stream.buffer != NULL) {
        UpdateMusicStream(g_backgroundMusic);
    }
    if (g_debugMusic.stream.buffer != NULL) {
        UpdateMusicStream(g_debugMusic);
    }
    
    // Update the main handler
    UpdateHandler2D();
}

// Render the game
void RenderGame(void) {
    // Render through the 2D handler
    RenderHandler2D();
}

// Shutdown all systems
void ShutdownGame(void) {
    printf("\nShutting down game systems...\n");
    
    // Shutdown 2D handler
    ShutdownHandler2D();
    
    // Shutdown screen manager
    UnloadScreenManager(&g_screenManager);
    
    // Shutdown asset manager
    UnloadAssetManager();
    
    // Stop and unload music
    if (g_backgroundMusic.stream.buffer != NULL) {
        StopMusicStream(g_backgroundMusic);
        UnloadMusicStream(g_backgroundMusic);
        printf("✓ Background music unloaded\n");
    }
    if (g_debugMusic.stream.buffer != NULL) {
        StopMusicStream(g_debugMusic);
        UnloadMusicStream(g_debugMusic);
        printf("✓ Debug music unloaded\n");
    }
    
    // Unload fonts
    UnloadFont(geetRegular);
    UnloadFont(malvidesRegular);
    UnloadFont(neu5landNormal);
    printf("✓ Fonts unloaded\n");
    
    // Cleanup raylib resources
    UnloadRenderTexture(g_virtualScreen);
    CloseAudioDevice();
    CloseWindow();
    
    printf("✓ All systems shut down\n");
}

// Implementation for getting mouse position in virtual screen coordinates
Vector2 GetMousePositionVirtual(void) {
    Vector2 mousePos = GetMousePosition();
    
    // Convert from window coordinates to virtual screen coordinates
    float scaleX = (float)VIRTUAL_SCREEN_WIDTH / (float)screenWidth;
    float scaleY = (float)VIRTUAL_SCREEN_HEIGHT / (float)screenHeight;
    
    Vector2 virtualPos = {
        mousePos.x * scaleX,
        mousePos.y * scaleY
    };
    
    return virtualPos;
}

// Music transition functions
void StartDebugMusic(void) {
    if (g_debugMusic.stream.buffer != NULL) {
        // Fade out main music
        SetMusicVolume(g_backgroundMusic, 0.0f);
        if (IsMusicStreamPlaying(g_backgroundMusic)) {
            PauseMusicStream(g_backgroundMusic);
        }
        
        // Start debug music
        SetMusicVolume(g_debugMusic, 0.3f);
        PlayMusicStream(g_debugMusic);
        printf("♪ Switched to debug music (Ricochet)\n");
    }
}

void StartMainMusic(void) {
    if (g_backgroundMusic.stream.buffer != NULL) {
        // Fade out debug music
        SetMusicVolume(g_debugMusic, 0.0f);
        if (IsMusicStreamPlaying(g_debugMusic)) {
            PauseMusicStream(g_debugMusic);
        }
        
        // Resume main music
        SetMusicVolume(g_backgroundMusic, 0.3f);
        ResumeMusicStream(g_backgroundMusic);
        printf("♪ Switched to main music (Rift)\n");
    }
}
