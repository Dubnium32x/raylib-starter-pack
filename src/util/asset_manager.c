// =============================================================
// Asset Manager Implementation
// =============================================================
// Manages loading and unloading of game assets

#include "asset_manager.h"
#include <string.h>
#include <stdio.h>

// Global asset manager instance
AssetManager g_assetManager = {0};

void InitAssetManager(void) {
    printf("Initializing Asset Manager...\n");
    
    memset(&g_assetManager, 0, sizeof(AssetManager));
    g_assetManager.initialized = true;
    
    printf("✓ Asset Manager initialized\n");
}

void UnloadAssetManager(void) {
    if (!g_assetManager.initialized) return;
    
    printf("Unloading Asset Manager...\n");
    
    // Unload all textures
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (g_assetManager.textureLoaded[i]) {
            UnloadTexture(g_assetManager.textures[i]);
        }
    }
    
    // Unload all sounds
    for (int i = 0; i < MAX_SOUNDS; i++) {
        if (g_assetManager.soundLoaded[i]) {
            UnloadSound(g_assetManager.sounds[i]);
        }
    }
    
    // Unload all music
    for (int i = 0; i < MAX_MUSIC; i++) {
        if (g_assetManager.musicLoaded[i]) {
            UnloadMusicStream(g_assetManager.music[i]);
        }
    }
    
    // Unload all fonts
    for (int i = 0; i < MAX_FONTS; i++) {
        if (g_assetManager.fontLoaded[i]) {
            UnloadFont(g_assetManager.fonts[i]);
        }
    }
    
    g_assetManager.initialized = false;
    printf("✓ Asset Manager unloaded\n");
}

// Texture management
int LoadAssetTexture(const char* name, const char* filePath) {
    if (!g_assetManager.initialized) return -1;
    
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (!g_assetManager.textureLoaded[i]) {
            g_assetManager.textures[i] = LoadTexture(filePath);
            if (g_assetManager.textures[i].id > 0) {
                strncpy(g_assetManager.textureNames[i], name, 63);
                g_assetManager.textureNames[i][63] = '\0';
                g_assetManager.textureLoaded[i] = true;
                printf("✓ Loaded texture: %s\n", name);
                return i;
            } else {
                printf("✗ Failed to load texture: %s (%s)\n", name, filePath);
                return -1;
            }
        }
    }
    
    printf("✗ No texture slots available for: %s\n", name);
    return -1;
}

Texture2D GetAssetTexture(const char* name) {
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (g_assetManager.textureLoaded[i] && strcmp(g_assetManager.textureNames[i], name) == 0) {
            return g_assetManager.textures[i];
        }
    }
    
    // Return empty texture if not found
    return (Texture2D){0};
}

// Sound management
int LoadAssetSound(const char* name, const char* filePath) {
    if (!g_assetManager.initialized) return -1;
    
    for (int i = 0; i < MAX_SOUNDS; i++) {
        if (!g_assetManager.soundLoaded[i]) {
            g_assetManager.sounds[i] = LoadSound(filePath);
            if (g_assetManager.sounds[i].stream.buffer != NULL) {
                strncpy(g_assetManager.soundNames[i], name, 63);
                g_assetManager.soundNames[i][63] = '\0';
                g_assetManager.soundLoaded[i] = true;
                printf("✓ Loaded sound: %s\n", name);
                return i;
            } else {
                printf("✗ Failed to load sound: %s (%s)\n", name, filePath);
                return -1;
            }
        }
    }
    
    printf("✗ No sound slots available for: %s\n", name);
    return -1;
}

Sound GetAssetSound(const char* name) {
    for (int i = 0; i < MAX_SOUNDS; i++) {
        if (g_assetManager.soundLoaded[i] && strcmp(g_assetManager.soundNames[i], name) == 0) {
            return g_assetManager.sounds[i];
        }
    }
    
    // Return empty sound if not found
    return (Sound){0};
}

void PlayAssetSound(const char* name) {
    Sound sound = GetAssetSound(name);
    if (sound.stream.buffer != NULL) {
        PlaySound(sound);
    }
}

// Music management
int LoadAssetMusic(const char* name, const char* filePath) {
    if (!g_assetManager.initialized) return -1;
    
    for (int i = 0; i < MAX_MUSIC; i++) {
        if (!g_assetManager.musicLoaded[i]) {
            g_assetManager.music[i] = LoadMusicStream(filePath);
            if (g_assetManager.music[i].stream.buffer != NULL) {
                strncpy(g_assetManager.musicNames[i], name, 63);
                g_assetManager.musicNames[i][63] = '\0';
                g_assetManager.musicLoaded[i] = true;
                printf("✓ Loaded music: %s\n", name);
                return i;
            } else {
                printf("✗ Failed to load music: %s (%s)\n", name, filePath);
                return -1;
            }
        }
    }
    
    printf("✗ No music slots available for: %s\n", name);
    return -1;
}

Music GetAssetMusic(const char* name) {
    for (int i = 0; i < MAX_MUSIC; i++) {
        if (g_assetManager.musicLoaded[i] && strcmp(g_assetManager.musicNames[i], name) == 0) {
            return g_assetManager.music[i];
        }
    }
    
    // Return empty music if not found
    return (Music){0};
}

void PlayAssetMusic(const char* name, bool loop) {
    Music music = GetAssetMusic(name);
    if (music.stream.buffer != NULL) {
        music.looping = loop;
        PlayMusicStream(music);
    }
}

void StopAssetMusic(const char* name) {
    Music music = GetAssetMusic(name);
    if (music.stream.buffer != NULL) {
        StopMusicStream(music);
    }
}

// Font management
int LoadAssetFont(const char* name, const char* filePath, int fontSize) {
    if (!g_assetManager.initialized) return -1;
    
    for (int i = 0; i < MAX_FONTS; i++) {
        if (!g_assetManager.fontLoaded[i]) {
            g_assetManager.fonts[i] = LoadFontEx(filePath, fontSize, 0, 250);
            if (g_assetManager.fonts[i].texture.id > 0) {
                strncpy(g_assetManager.fontNames[i], name, 63);
                g_assetManager.fontNames[i][63] = '\0';
                g_assetManager.fontLoaded[i] = true;
                printf("✓ Loaded font: %s\n", name);
                return i;
            } else {
                printf("✗ Failed to load font: %s (%s)\n", name, filePath);
                return -1;
            }
        }
    }
    
    printf("✗ No font slots available for: %s\n", name);
    return -1;
}

Font GetAssetFont(const char* name) {
    for (int i = 0; i < MAX_FONTS; i++) {
        if (g_assetManager.fontLoaded[i] && strcmp(g_assetManager.fontNames[i], name) == 0) {
            return g_assetManager.fonts[i];
        }
    }
    
    // Return default font if not found
    return GetFontDefault();
}