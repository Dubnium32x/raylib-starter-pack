// =============================================================
// Screen Manager source
// =============================================================
// Implements functions to manage different game screens and transitions

#include "screen_manager.h"
#include "screen_state.h"
#include <string.h>

// Global screen manager instance - external reference
extern ScreenManager g_screenManager;

// Initialize the Screen Manager
void InitScreenManager(ScreenManager *manager) {

    memset(manager->screens, 0, sizeof(manager->screens));
    manager->currentScreen = SCREEN_STATE_INIT;
    manager->initialized = true;
}

// Register a screen with its associated functions
void RegisterScreen(ScreenManager *manager, ScreenType type, 
                   void (*Init)(void), 
                   void (*Update)(float), 
                   void (*Draw)(void), 
                   void (*Unload)(void)) {
    if (type < 0 || type >= MAX_SCREENS) return;
    manager->screens[type].Init = Init;
    manager->screens[type].Update = Update;
    manager->screens[type].Draw = Draw;
    manager->screens[type].Unload = Unload;
}

// Set the current active screen
void SetCurrentScreen(ScreenManager *manager, ScreenType type) {
    if (manager == NULL || type < 0 || type >= MAX_SCREENS) return;

    // Unload current screen if it has an Unload function
    if (manager->screens[manager->currentScreen].Unload != NULL) {
        manager->screens[manager->currentScreen].Unload();
    }

    manager->currentScreen = type;

    // Initialize new screen if it has an Init function
    if (manager->screens[type].Init != NULL) {
        manager->screens[type].Init();
    }
}

// Update the current screen
void UpdateScreenManager(ScreenManager *manager, float deltaTime) {
    if (manager == NULL) return;

    // Call the Update function of the current screen
    if (manager->screens[manager->currentScreen].Update != NULL) {
        manager->screens[manager->currentScreen].Update(deltaTime);
    }
}

// Draw the current screen

void DrawScreenManager(ScreenManager *manager) {
    if (manager == NULL) return;

    // Call the Draw function of the current screen
    if (manager->screens[manager->currentScreen].Draw != NULL) {
        manager->screens[manager->currentScreen].Draw();
    }
}

// Unload all screens in the manager
void UnloadScreenManager(ScreenManager *manager) {
    if (manager == NULL) return;

    // Unload all registered screens
    for (int i = 0; i < MAX_SCREENS; i++) {
        if (manager->screens[i].Unload != NULL) {
            manager->screens[i].Unload();
        }
    }

    manager->initialized = false;
}

// Global helper to switch screens from anywhere after InitScreenManager
void SetCurrentScreenGlobal(ScreenType type) {
    // Use external global screen manager
    if (g_screenManager.initialized) {
        SetCurrentScreen(&g_screenManager, type);
    }
}
