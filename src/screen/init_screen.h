// =============================================================
// Init screen header
// =============================================================
// This screen handles the initialization phase of the game,
// setting up necessary resources and configurations before
// transitioning to the main menu or gameplay screen.
// This is also the first screen that loads when the game starts.
// =============================================================

#ifndef INIT_SCREEN_H
#define INIT_SCREEN_H

#include "raylib.h"

// Function declarations
void InitScreen_Init(void);
void InitScreen_Update(float deltaTime);
void InitScreen_Draw(void);
void InitScreen_Unload(void);

#endif // INIT_SCREEN_H
