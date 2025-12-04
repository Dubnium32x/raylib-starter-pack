// =============================================================
// Asset Manager Header
// =============================================================
// Manages loading and unloading of game assets
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_TEXTURES 32
#define MAX_SOUNDS 16
#define MAX_MUSIC 4
#define MAX_FONTS 8

typedef struct {
    Texture2D textures[MAX_TEXTURES];
    Sound sounds[MAX_SOUNDS];
    Music music[MAX_MUSIC];
    Font fonts[MAX_FONTS];
    
    char textureNames[MAX_TEXTURES][64];
    char soundNames[MAX_SOUNDS][64];
    char musicNames[MAX_MUSIC][64];
    char fontNames[MAX_FONTS][64];
    
    bool textureLoaded[MAX_TEXTURES];
    bool soundLoaded[MAX_SOUNDS];
    bool musicLoaded[MAX_MUSIC];
    bool fontLoaded[MAX_FONTS];
    
    bool initialized;
} AssetManager;

// Global asset manager
extern AssetManager g_assetManager;

// Function prototypes
void InitAssetManager(void);
void UnloadAssetManager(void);

// Texture management
int LoadAssetTexture(const char* name, const char* filePath);
Texture2D GetAssetTexture(const char* name);
void UnloadAssetTexture(const char* name);

// Sound management
int LoadAssetSound(const char* name, const char* filePath);
Sound GetAssetSound(const char* name);
void PlayAssetSound(const char* name);
void UnloadAssetSound(const char* name);

// Music management
int LoadAssetMusic(const char* name, const char* filePath);
Music GetAssetMusic(const char* name);
void PlayAssetMusic(const char* name, bool loop);
void StopAssetMusic(const char* name);
void UnloadAssetMusic(const char* name);

// Font management
int LoadAssetFont(const char* name, const char* filePath, int fontSize);
Font GetAssetFont(const char* name);
void UnloadAssetFont(const char* name);

#endif // ASSET_MANAGER_H