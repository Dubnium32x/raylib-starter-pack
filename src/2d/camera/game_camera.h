// =============================================================
// Game Camera Header
// =============================================================
// Simple 2D camera system for the framework
#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include "raylib.h"

typedef struct {
    Camera2D camera;
    Vector2 target;
    Vector2 offset;
    float rotation;
    float zoom;
    bool followingTarget;
    Vector2 smoothing;
} GameCamera;

// Function prototypes
GameCamera InitGameCamera(Vector2 target, Vector2 offset, float rotation, float zoom);
void UpdateGameCamera(GameCamera* camera, Vector2 newTarget, float deltaTime);
void SetGameCameraTarget(GameCamera* camera, Vector2 target);
void SetGameCameraOffset(GameCamera* camera, Vector2 offset);
void SetGameCameraZoom(GameCamera* camera, float zoom);
void BeginGameCameraMode(GameCamera* camera);
void EndGameCameraMode(void);

#endif // GAME_CAMERA_H