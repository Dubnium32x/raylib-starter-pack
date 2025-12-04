// =============================================================
// Debug Test 2 - Top-Down Shooter Test
// =============================================================
// Demonstrates basic top-down shooter mechanics with AI,
// player movement, shooting, and collision detection

#include "debug_test_2.h"
#include "../util/globals.h"
#include "../world/screen_manager.h"
#include "../2d/handler2d.h"
#include "../world/input_manager.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Asset manager for sprites and sounds
#include "../util/asset_manager.h"

#define MAX_BULLETS 50
#define MAX_ENEMIES 10
#define MAX_PARTICLES 100

// Bullet structure
typedef struct {
    Vector2 position;
    Vector2 velocity;
    float lifetime;
    bool active;
    Color color;
} Bullet;

// Enemy structure
typedef struct {
    Vector2 position;
    Vector2 velocity;
    float health;
    float speed;
    bool active;
    Color color;
    float aiTimer;
} Enemy;

// Particle structure
typedef struct {
    Vector2 position;
    Vector2 velocity;
    float lifetime;
    float maxLifetime;
    bool active;
    Color color;
} Particle;

// Player structure
typedef struct {
    Vector2 position;
    float rotation;
    Vector2 velocity;
    float health;
    float maxHealth;
    Color color;
    float shootCooldown;
} TopDownPlayer;

// Test screen state
static TopDownPlayer player = {0};
static Bullet bullets[MAX_BULLETS] = {0};
static Enemy enemies[MAX_ENEMIES] = {0};
static Particle particles[MAX_PARTICLES] = {0};
static bool initialized = false;
static float enemySpawnTimer = 0.0f;
static float enemySpawnInterval = 2.0f;
static int score = 0;
static Vector2 cameraOffset = {0};

// Textures and sounds
static Texture2D playerTexture;
static Texture2D enemyTexture;
static Sound shootSound;
static Sound explosionSound;

// Helper functions
static void SpawnEnemy(void);
static void FireBullet(Vector2 position, Vector2 direction);
static void CreateParticleExplosion(Vector2 position, Color color);
static Vector2 Vector2Normalize(Vector2 v);
static float Vector2Distance(Vector2 v1, Vector2 v2);

void DebugTest2_Init(void) {
    printf("Initializing Debug Test 2 (Top-Down Shooter)...\n");
    
    // Switch to debug music
    extern void StartDebugMusic(void);
    StartDebugMusic();
    
    // Initialize player
    player.position = (Vector2){VIRTUAL_SCREEN_WIDTH/2, VIRTUAL_SCREEN_HEIGHT/2};
    player.rotation = 0.0f;
    player.velocity = (Vector2){0, 0};
    player.health = 100.0f;
    player.maxHealth = 100.0f;
    player.color = GREEN;
    player.shootCooldown = 0.0f;
    
    // Load player texture from PNG file
    playerTexture = LoadTexture("res/image/player_shooter.png");
    
    // Load enemy texture from PNG file
    enemyTexture = LoadTexture("res/image/enemy.png");
    
    // Load sound effects from files
    shootSound = LoadSound("res/audio/sfx/shot.wav");
    explosionSound = LoadSound("res/audio/sfx/flowerhit.wav");
    
    // Clear arrays
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
    }
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].active = false;
    }
    
    // Reset game state
    enemySpawnTimer = 0.0f;
    score = 0;
    cameraOffset = (Vector2){0, 0};
    
    initialized = true;
    printf("✓ Debug Test 2 ready\n");
}

void DebugTest2_Update(float deltaTime) {
    if (!initialized) return;
    
    // Input handling
    Vector2 moveInput = {0, 0};
    
    // Movement input
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) moveInput.y = -1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) moveInput.y = 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) moveInput.x = -1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveInput.x = 1.0f;
    
    // Normalize movement
    if (moveInput.x != 0 || moveInput.y != 0) {
        moveInput = Vector2Normalize(moveInput);
    }
    
    // Apply movement
    float moveSpeed = 250.0f;
    player.velocity = (Vector2){moveInput.x * moveSpeed, moveInput.y * moveSpeed};
    player.position.x += player.velocity.x * deltaTime;
    player.position.y += player.velocity.y * deltaTime;
    
    // Keep player on screen
    if (player.position.x < 15) player.position.x = 15;
    if (player.position.x > VIRTUAL_SCREEN_WIDTH - 15) player.position.x = VIRTUAL_SCREEN_WIDTH - 15;
    if (player.position.y < 15) player.position.y = 15;
    if (player.position.y > VIRTUAL_SCREEN_HEIGHT - 15) player.position.y = VIRTUAL_SCREEN_HEIGHT - 15;
    
    // Mouse aim and rotation
    Vector2 mousePos = GetMousePositionVirtual();
    Vector2 aimDir = {mousePos.x - player.position.x, mousePos.y - player.position.y};
    // Adjust rotation by -90 degrees to align sprite properly (assuming sprite faces up by default)
    player.rotation = atan2f(aimDir.y, aimDir.x) * RAD2DEG + 90.0f;
    
    // Shooting
    if (player.shootCooldown > 0.0f) {
        player.shootCooldown -= deltaTime;
    }
    
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && player.shootCooldown <= 0.0f) {
        Vector2 shootDir = Vector2Normalize(aimDir);
        
        // Calculate bullet spawn position at front of player
        Vector2 bulletSpawnPos = {
            player.position.x + shootDir.x * 20.0f,  // Offset forward from player center
            player.position.y + shootDir.y * 20.0f
        };
        
        FireBullet(bulletSpawnPos, shootDir);
        player.shootCooldown = 0.15f;
    }
    
    // Update bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        
        bullets[i].position.x += bullets[i].velocity.x * deltaTime;
        bullets[i].position.y += bullets[i].velocity.y * deltaTime;
        bullets[i].lifetime -= deltaTime;
        
        if (bullets[i].lifetime <= 0.0f || 
            bullets[i].position.x < 0 || bullets[i].position.x > VIRTUAL_SCREEN_WIDTH ||
            bullets[i].position.y < 0 || bullets[i].position.y > VIRTUAL_SCREEN_HEIGHT) {
            bullets[i].active = false;
        }
    }
    
    // Spawn enemies
    enemySpawnTimer += deltaTime;
    if (enemySpawnTimer >= enemySpawnInterval) {
        SpawnEnemy();
        enemySpawnTimer = 0.0f;
    }
    
    // Update enemies
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;
        
        // AI - move towards player
        Vector2 toPlayer = {player.position.x - enemies[i].position.x, 
                           player.position.y - enemies[i].position.y};
        toPlayer = Vector2Normalize(toPlayer);
        
        enemies[i].velocity.x = toPlayer.x * enemies[i].speed;
        enemies[i].velocity.y = toPlayer.y * enemies[i].speed;
        
        enemies[i].position.x += enemies[i].velocity.x * deltaTime;
        enemies[i].position.y += enemies[i].velocity.y * deltaTime;
        
        // Check collision with player
        if (Vector2Distance(enemies[i].position, player.position) < 25.0f) {
            player.health -= 20.0f * deltaTime;
            CreateParticleExplosion(enemies[i].position, RED);
        }
        
        // Check bullet collisions
        for (int j = 0; j < MAX_BULLETS; j++) {
            if (!bullets[j].active) continue;
            
            if (Vector2Distance(bullets[j].position, enemies[i].position) < 15.0f) {
                // Hit enemy
                CreateParticleExplosion(enemies[i].position, enemies[i].color);
                PlaySound(explosionSound);
                enemies[i].active = false;
                bullets[j].active = false;
                score += 10;
                break;
            }
        }
    }
    
    // Update particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;
        
        particles[i].position.x += particles[i].velocity.x * deltaTime;
        particles[i].position.y += particles[i].velocity.y * deltaTime;
        particles[i].lifetime -= deltaTime;
        
        if (particles[i].lifetime <= 0.0f) {
            particles[i].active = false;
        }
    }
    
    // Game over check
    if (player.health <= 0.0f) {
        // Reset game
        DebugTest2_Init();
    }
    
    // Exit back to title screen
    if (IsKeyPressed(KEY_ESCAPE)) {
        SetScreen(SCREEN_STATE_TITLE);
    }
    
    // Switch to next test
    if (IsKeyPressed(KEY_TAB)) {
        SetScreen(SCREEN_STATE_DEBUG3);
    }
}

void DebugTest2_Draw(void) {
    if (!initialized) return;
    
    // Background
    ClearBackground(DARKGRAY);
    
    // Draw grid
    for (int i = 0; i < VIRTUAL_SCREEN_WIDTH; i += 50) {
        DrawLine(i, 0, i, VIRTUAL_SCREEN_HEIGHT, GRAY);
    }
    for (int i = 0; i < VIRTUAL_SCREEN_HEIGHT; i += 50) {
        DrawLine(0, i, VIRTUAL_SCREEN_WIDTH, i, GRAY);
    }
    
    // Draw particles (behind everything)
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;
        
        float alpha = particles[i].lifetime / particles[i].maxLifetime;
        Color color = particles[i].color;
        color.a = (unsigned char)(255 * alpha);
        
        DrawCircleV(particles[i].position, 3.0f, color);
    }
    
    // Draw enemies with texture
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;
        
        Rectangle destRec = {enemies[i].position.x - 15, enemies[i].position.y - 15, 30, 30};
        Rectangle sourceRec = {0, 0, enemyTexture.width, enemyTexture.height};
        Vector2 origin = {0, 0};
        DrawTexturePro(enemyTexture, sourceRec, destRec, origin, 0, WHITE);
    }
    
    // Draw bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        
        DrawCircleV(bullets[i].position, 3.0f, bullets[i].color);
    }
    
    // Draw player with texture
    Rectangle destRec = {player.position.x - 16, player.position.y - 16, 32, 32};
    Rectangle sourceRec = {0, 0, playerTexture.width, playerTexture.height};
    Vector2 origin = {16, 16};
    DrawTexturePro(playerTexture, sourceRec, destRec, origin, player.rotation, WHITE);
    
    // UI Elements
    DrawText("DEBUG TEST 2: TOP-DOWN SHOOTER", 10, 10, 20, WHITE);
    DrawText("WASD: Move", 10, 40, 16, WHITE);
    DrawText("Mouse: Aim, Left Click: Shoot", 10, 60, 16, WHITE);
    DrawText("TAB: Next Test", 10, 80, 16, WHITE);
    DrawText("ESC: Back to Title", 10, 100, 16, WHITE);
    
    // Game stats
    DrawText(TextFormat("Score: %d", score), 10, 140, 16, YELLOW);
    DrawText(TextFormat("Health: %.0f/%.0f", player.health, player.maxHealth), 10, 160, 16, YELLOW);
    
    // Health bar
    Rectangle healthBarBG = {10, 180, 200, 20};
    Rectangle healthBar = {10, 180, (player.health / player.maxHealth) * 200, 20};
    DrawRectangleRec(healthBarBG, MAROON);
    DrawRectangleRec(healthBar, RED);
    DrawRectangleLinesEx(healthBarBG, 2, WHITE);
}

void DebugTest2_Unload(void) {
    printf("Unloading Debug Test 2...\n");
    
    // Switch back to main music
    extern void StartMainMusic(void);
    StartMainMusic();
    
    // Unload textures
    if (playerTexture.id > 0) {
        UnloadTexture(playerTexture);
    }
    if (enemyTexture.id > 0) {
        UnloadTexture(enemyTexture);
    }
    
    // Unload sounds
    UnloadSound(shootSound);
    UnloadSound(explosionSound);
    
    initialized = false;
}

// Helper function implementations
static void SpawnEnemy(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            // Spawn at random edge
            int edge = GetRandomValue(0, 3);
            switch (edge) {
                case 0: // Top
                    enemies[i].position = (Vector2){GetRandomValue(0, VIRTUAL_SCREEN_WIDTH), -20};
                    break;
                case 1: // Right
                    enemies[i].position = (Vector2){VIRTUAL_SCREEN_WIDTH + 20, GetRandomValue(0, VIRTUAL_SCREEN_HEIGHT)};
                    break;
                case 2: // Bottom
                    enemies[i].position = (Vector2){GetRandomValue(0, VIRTUAL_SCREEN_WIDTH), VIRTUAL_SCREEN_HEIGHT + 20};
                    break;
                case 3: // Left
                    enemies[i].position = (Vector2){-20, GetRandomValue(0, VIRTUAL_SCREEN_HEIGHT)};
                    break;
            }
            
            enemies[i].velocity = (Vector2){0, 0};
            enemies[i].health = 1.0f;
            enemies[i].speed = GetRandomValue(80, 120);
            enemies[i].active = true;
            enemies[i].color = RED;
            enemies[i].aiTimer = 0.0f;
            break;
        }
    }
}

static void FireBullet(Vector2 position, Vector2 direction) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].position = position;
            bullets[i].velocity = (Vector2){direction.x * 500.0f, direction.y * 500.0f};
            bullets[i].lifetime = 3.0f;
            bullets[i].active = true;
            bullets[i].color = YELLOW;
            PlaySound(shootSound);
            break;
        }
    }
}

static void CreateParticleExplosion(Vector2 position, Color color) {
    int particlesToSpawn = 8;
    for (int i = 0; i < particlesToSpawn; i++) {
        for (int j = 0; j < MAX_PARTICLES; j++) {
            if (!particles[j].active) {
                float angle = (float)i / particlesToSpawn * 360.0f * DEG2RAD;
                float speed = GetRandomValue(50, 150);
                
                particles[j].position = position;
                particles[j].velocity = (Vector2){cosf(angle) * speed, sinf(angle) * speed};
                particles[j].lifetime = GetRandomValue(50, 150) / 100.0f;
                particles[j].maxLifetime = particles[j].lifetime;
                particles[j].active = true;
                particles[j].color = color;
                break;
            }
        }
    }
}

static Vector2 Vector2Normalize(Vector2 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y);
    if (length > 0.0f) {
        v.x /= length;
        v.y /= length;
    }
    return v;
}

static float Vector2Distance(Vector2 v1, Vector2 v2) {
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    return sqrtf(dx * dx + dy * dy);
}