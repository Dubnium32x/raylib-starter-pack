// =============================================================
// Debug Test 1 - 2D Platformer Test
// =============================================================
// Demonstrates basic 2D platformer mechanics with physics,
// player movement, and collision detection

#include "debug_test_1.h"
#include "../util/globals.h"
#include "../world/screen_manager.h"
#include "../2d/handler2d.h"
#include "../world/input_manager.h"
#include <stdio.h>
#include <math.h>

// Asset manager for sprites and sounds
#include "../util/asset_manager.h"

// Player structure
typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    Color color;
    bool onGround;
    bool facingRight;
} Player;

// Platform structure
typedef struct {
    Rectangle rect;
    Color color;
} Platform;

// Test screen state
static Player player = {0};
static Platform platforms[10] = {0};
static int platformCount = 0;
static bool initialized = false;
static float gravity = 800.0f;
static float jumpForce = 400.0f;
static float moveSpeed = 200.0f;
static Vector2 cameraOffset = {0};
static Texture2D playerTexture;
static Sound jumpSound;
static Sound landSound;
static bool wasOnGround = false;

void DebugTest1_Init(void) {
    printf("Initializing Debug Test 1 (2D Platformer)...\n");
    
    // Switch to debug music
    extern void StartDebugMusic(void);
    StartDebugMusic();
    
    // Create simple player texture (32x32 blue square with border)
    Image playerImage = GenImageColor(32, 32, BLUE);
    Rectangle borderRec = {0, 0, 32, 32};
    ImageDrawRectangleLines(&playerImage, borderRec, 2, DARKBLUE);
    ImageDrawRectangle(&playerImage, 12, 8, 8, 8, WHITE); // Eyes
    ImageDrawRectangle(&playerImage, 8, 20, 16, 4, DARKBLUE); // Mouth
    playerTexture = LoadTextureFromImage(playerImage);
    UnloadImage(playerImage);
    
    // Load sound effects from files
    jumpSound = LoadSound("res/audio/sfx/jump2.wav");
    landSound = LoadSound("res/audio/sfx/flipstep.wav"); // Using flipstep as landing sound
    
    // Store the texture (we'll manually manage this one)
    // In a real game, you'd use LoadAssetTexture with actual files
    
    // Initialize player
    player.position = (Vector2){100, 300};
    player.velocity = (Vector2){0, 0};
    player.size = (Vector2){32, 32};
    player.color = WHITE; // Use white to show texture properly
    player.onGround = false;
    player.facingRight = true;
    
    // Initialize platforms
    platformCount = 6;
    
    // Ground platform
    platforms[0] = (Platform){{0, 500, 800, 50}, GREEN};
    
    // Floating platforms
    platforms[1] = (Platform){{200, 400, 100, 20}, GREEN};
    platforms[2] = (Platform){{400, 350, 100, 20}, GREEN};
    platforms[3] = (Platform){{600, 300, 100, 20}, GREEN};
    platforms[4] = (Platform){{300, 250, 150, 20}, GREEN};
    platforms[5] = (Platform){{100, 150, 120, 20}, GREEN};
    
    // Initialize camera
    cameraOffset = (Vector2){0, 0};
    
    initialized = true;
    printf("✓ Debug Test 1 ready\n");
}

void DebugTest1_Update(float deltaTime) {
    if (!initialized) return;
    
    // Store previous ground state for landing detection
    wasOnGround = player.onGround;
    
    // Input handling
    Vector2 input = {0, 0};
    
    // Movement input
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        input.x = -1.0f;
        player.facingRight = false;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        input.x = 1.0f;
        player.facingRight = true;
    }
    
    // Jump input
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && player.onGround) {
        player.velocity.y = -jumpForce;
        player.onGround = false;
        PlaySound(jumpSound);
    }
    
    // Apply horizontal movement
    player.velocity.x = input.x * moveSpeed;
    
    // Apply gravity
    player.velocity.y += gravity * deltaTime;
    
    // Apply velocity to position
    player.position.x += player.velocity.x * deltaTime;
    player.position.y += player.velocity.y * deltaTime;
    
    // Check platform collisions
    Rectangle playerRect = {player.position.x, player.position.y, player.size.x, player.size.y};
    player.onGround = false;
    
    for (int i = 0; i < platformCount; i++) {
        if (CheckCollisionRecs(playerRect, platforms[i].rect)) {
            // Top collision (landing on platform)
            if (player.velocity.y > 0 && 
                player.position.y < platforms[i].rect.y) {
                player.position.y = platforms[i].rect.y - player.size.y;
                player.velocity.y = 0;
                
                // Play landing sound if we just landed
                if (!wasOnGround) {
                    PlaySound(landSound);
                }
                player.onGround = true;
            }
            // Bottom collision (hitting platform from below)
            else if (player.velocity.y < 0 && 
                     player.position.y > platforms[i].rect.y) {
                player.position.y = platforms[i].rect.y + platforms[i].rect.height;
                player.velocity.y = 0;
            }
        }
    }
    
    // Update camera to follow player
    cameraOffset.x = VIRTUAL_SCREEN_WIDTH/2;
    cameraOffset.y = VIRTUAL_SCREEN_HEIGHT/2;
    
    // Reset player if they fall off the world
    if (player.position.y > 700) {
        player.position = (Vector2){100, 300};
        player.velocity = (Vector2){0, 0};
        cameraOffset = (Vector2){0, 0};
    }
    
    // Exit back to title screen
    if (IsKeyPressed(KEY_ESCAPE)) {
        SetScreen(SCREEN_STATE_TITLE);
    }
    
    // Switch to next test
    if (IsKeyPressed(KEY_TAB)) {
        SetScreen(SCREEN_STATE_DEBUG2);
    }
}

void DebugTest1_Draw(void) {
    if (!initialized) return;
    
    // Background
    ClearBackground(SKYBLUE);
    
    // Apply camera - target follows player, offset centers the view
    Camera2D camera = {
        .target = (Vector2){player.position.x + player.size.x/2, player.position.y + player.size.y/2},
        .offset = cameraOffset,
        .rotation = 0.0f,
        .zoom = 1.0f
    };
    BeginMode2D(camera);
    
    // Draw platforms
    for (int i = 0; i < platformCount; i++) {
        DrawRectangleRec(platforms[i].rect, platforms[i].color);
        DrawRectangleLinesEx(platforms[i].rect, 2, DARKGREEN);
    }
    
    // Draw player with texture
    Rectangle destRec = {player.position.x, player.position.y, player.size.x, player.size.y};
    Rectangle sourceRec = {0, 0, playerTexture.width, playerTexture.height};
    
    // Flip texture if facing left
    if (!player.facingRight) {
        sourceRec.width = -playerTexture.width;
    }
    
    DrawTexturePro(playerTexture, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
    
    // Draw player direction indicator
    if (player.facingRight) {
        DrawTriangle((Vector2){player.position.x + player.size.x - 5, player.position.y + 10},
                    (Vector2){player.position.x + player.size.x - 5, player.position.y + 22},
                    (Vector2){player.position.x + player.size.x + 3, player.position.y + 16},
                    YELLOW);
    } else {
        DrawTriangle((Vector2){player.position.x + 5, player.position.y + 10},
                    (Vector2){player.position.x + 5, player.position.y + 22},
                    (Vector2){player.position.x - 3, player.position.y + 16},
                    YELLOW);
    }
    
    EndMode2D();
    
    // UI Elements
    DrawText("DEBUG TEST 1: 2D PLATFORMER", 10, 10, 20, WHITE);
    DrawText("WASD/Arrow Keys: Move", 10, 40, 16, WHITE);
    DrawText("SPACE: Jump", 10, 60, 16, WHITE);
    DrawText("TAB: Next Test", 10, 80, 16, WHITE);
    DrawText("ESC: Back to Title", 10, 100, 16, WHITE);
    
    // Player info
    DrawText(TextFormat("Position: (%.1f, %.1f)", player.position.x, player.position.y), 10, 140, 14, YELLOW);
    DrawText(TextFormat("Velocity: (%.1f, %.1f)", player.velocity.x, player.velocity.y), 10, 160, 14, YELLOW);
    DrawText(TextFormat("On Ground: %s", player.onGround ? "YES" : "NO"), 10, 180, 14, YELLOW);
    DrawText(TextFormat("Facing: %s", player.facingRight ? "RIGHT" : "LEFT"), 10, 200, 14, YELLOW);
}

void DebugTest1_Unload(void) {
    printf("Unloading Debug Test 1...\n");
    
    // Switch back to main music
    extern void StartMainMusic(void);
    StartMainMusic();
    
    // Unload texture
    if (playerTexture.id > 0) {
        UnloadTexture(playerTexture);
    }
    
    // Unload sounds
    UnloadSound(jumpSound);
    UnloadSound(landSound);
    
    initialized = false;
}