// =============================================================
// Screen Manager header
// =============================================================
// Defines the Screen Manager for handling different game screens
#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H
#include "raylib.h"
#include "../util/globals.h"
#include "screen_state.h"

#define MAX_SCREENS 32

// Pseudo interface for a screen
typedef struct {
    void (*Init)(void);
    void (*Update)(float deltaTime);
    void (*Draw)(void);
    void (*Unload)(void);
} IScreen;

// Screen Manager structure
typedef struct {
    IScreen screens[MAX_SCREENS];
    ScreenType currentScreen;
    bool initialized;
} ScreenManager;

extern bool screenManagerInitialized;

// Function prototypes
void InitScreenManager(ScreenManager *manager);
void RegisterScreen(ScreenManager *manager, ScreenType type, 
                   void (*Init)(void), 
                   void (*Update)(float), 
                   void (*Draw)(void), 
                   void (*Unload)(void));
void SetCurrentScreen(ScreenManager *manager, ScreenType type);
void UpdateScreenManager(ScreenManager *manager, float deltaTime);
void DrawScreenManager(ScreenManager *manager);
void UnloadScreenManager(ScreenManager *manager);

// Global helper to switch screens from anywhere after InitScreenManager
void SetCurrentScreenGlobal(ScreenType type);

#endif // SCREEN_MANAGER_H