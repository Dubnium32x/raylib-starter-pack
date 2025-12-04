// =============================================================
// Screen state source
// =============================================================
// Implements functions to manage screen states in the game
#include "screen_state.h"

// Initialize the screen state with default values 
void InitScreenState(ScreenState* state) {
    state->currentScreen = SCREEN_STATE_INIT;
    state->previousScreen = SCREEN_STATE_INIT;
    state->currentMenu = OPTIONS;
    state->gameplayState = GAMEPLAY_PLAYING;
    state->initialized = false;
    state->frameCounter = 0;
    state->isPaused = false;
}

// Update the screen state
void UpdateScreenState(ScreenState* state, float deltaTime) {
    (void)deltaTime; // Suppress unused parameter warning
    state->frameCounter++;

    if (state->isPaused) {
        return; // Skip updates when paused
    }

    // Example screen state logic
    switch (state->currentScreen) {
        case SCREEN_STATE_INIT:
            // Transition to title screen after initialization
            if (state->frameCounter > 1) {
                state->previousScreen = state->currentScreen;
                state->currentScreen = SCREEN_STATE_TITLE;
                state->frameCounter = 0;
            }
            break;
        // Add cases for other screen states as needed
        default:
            break;
    }
}

// Change the current screen
void ChangeScreen(ScreenState* state, ScreenType newScreen) {
    if (!state) return;

    printf("Changing screen from %d to %d\n", state->currentScreen, newScreen);
    state->previousScreen = state->currentScreen;
    state->currentScreen = newScreen;
    state->frameCounter = 0;
}