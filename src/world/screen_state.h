// =============================================================
// Screen States header
// =============================================================
// These are the different screen states the game can be in
#ifndef SCREEN_STATES_H
#define SCREEN_STATES_H
#include <raylib.h>
#include "../util/globals.h"

// Screen state definitions
typedef enum {
    SCREEN_STATE_INIT,
    SCREEN_STATE_SPLASH,
    SCREEN_STATE_INTRO,
    SCREEN_STATE_TITLE,
    SCREEN_STATE_MAIN_MENU,
    SCREEN_STATE_OPTIONS,
    SCREEN_STATE_CREDITS,
    SCREEN_STATE_GAMEPLAY,
    SCREEN_STATE_PAUSE_MENU,
    SCREEN_STATE_GAME_OVER,
    SCREEN_STATE_LEVEL_COMPLETE,
    SCREEN_STATE_EXIT,

    // DEBUG SCREENS
    SCREEN_STATE_DEBUG1,
    SCREEN_STATE_DEBUG2,
    SCREEN_STATE_DEBUG3,
    SCREEN_STATE_DEBUG4,
    SCREEN_STATE_DEBUG5,
    SCREEN_STATE_DEBUG6,
    SCREEN_STATE_DEBUG7,
    SCREEN_STATE_DEBUG8
} ScreenType;

// Menu types
typedef enum {
    OPTIONS,
    FILE_SELECT,
    GALLERY,
    HELP,
    CREDITS
} MenuType;

// Gameplay states
typedef enum {
    GAMEPLAY_PLAYING,
    GAMEPLAY_PAUSED,
    GAMEPLAY_GAMEOVER,
    GAMEPLAY_CUTSCENE,
    GAMEPLAY_WIN
} GameplayState;

// Screen state structure
typedef struct {
    ScreenType currentScreen;
    ScreenType previousScreen;
    MenuType currentMenu;
    GameplayState gameplayState;
    bool initialized;
    int frameCounter;
    bool isPaused;
} ScreenState;

// Function prototypes
void InitScreenState(ScreenState* state);
void UpdateScreenState(ScreenState* state, float deltaTime);
void ChangeScreen(ScreenState* state, ScreenType newScreen);

#endif // SCREEN_STATES_H