// =============================================================
// Audio Manager source
// =============================================================
// This file is part of the manager instances in globals.h

#include "audio_manager.h"
#include <string.h>
#include <stdio.h>

AudioManager* g_audioManager = NULL;

// Init the audio manager
void InitAudioManager(AudioManager* manager) {
    if (manager == NULL) return;
    manager->sound_count = 0;
    manager->music_count = 0;
    manager->masterVolume = 1.0f;
    manager->musicVolume = 1.0f;
    manager->sfxVolume = 1.0f;
    manager->voxVolume = 1.0f;
    manager->ambienceVolume = 0.7f;
    manager->isMusicPlaying = false;
    manager->isMusicEnabled = true;
    manager->isSfxEnabled = true;
    manager->isVoxEnabled = true;
    manager->isAmbienceEnabled = true;
    manager->isFadingOut = false;
    manager->fadeOutDuration = 0.0f;
    manager->fadeOutTimer = 0.0f;
    manager->originalVolume = 1.0f;
    manager->pendingMusicPath[0] = '\0';
    manager->pendingMusicVolume = -1.0f;
    manager->pendingMusicLoop = true;
    manager->pendingMusicDelay = 0.0f;
    manager->pendingMusicDelayTimer = 0.0f;
    manager->isPendingMusicDelayed = false;

    // Get ready for sound and music loading
    for (int i = 0; i < MAX_SOUNDS; i++) {
        manager->sounds[i].type = SFX; // Default type
        manager->sounds[i].sound = (Sound){ 0 };
        manager->sounds[i].music = (Music){ 0 };
    }
    for (int i = 0; i < MAX_MUSIC_TRACKS; i++) {
        manager->music[i].type = MUSIC; // Default type
        manager->music[i].sound = (Sound){ 0 };
        manager->music[i].music = (Music){ 0 };
    }
}

// Update the audio manager (handle fading, etc.)
void UpdateAudioManager(AudioManager* manager, float deltaTime) {
    if (manager == NULL) return;

    // Update fading out music
    if (manager->isFadingOut) {
        manager->fadeOutTimer += deltaTime;
        float fadeProgress = manager->fadeOutTimer / manager->fadeOutDuration;
        if (fadeProgress >= 1.0f) {
            // Fade out complete
            StopMusicStream(manager->music[0].music);
            manager->isMusicPlaying = false;
            manager->isFadingOut = false;

            // Start pending music if any
            if (manager->pendingMusicPath[0] != '\0') {
                GamePlayMusic(manager, manager->pendingMusicPath, manager->pendingMusicVolume, manager->pendingMusicLoop, 0.0f);
                manager->pendingMusicPath[0] = '\0'; // Clear pending music
            }
        } else {
            // Continue fading out
            float newVolume = manager->originalVolume * (1.0f - fadeProgress);
            SetMusicVolume(manager->music[0].music, newVolume * manager->masterVolume);
        }
    }

    // Update pending music delay
    if (manager->isPendingMusicDelayed) {
        manager->pendingMusicDelayTimer += deltaTime;
        if (manager->pendingMusicDelayTimer >= manager->pendingMusicDelay) {
            // Delay complete, play pending music
            GamePlayMusic(manager, manager->pendingMusicPath, manager->pendingMusicVolume, manager->pendingMusicLoop, 0.0f);
            manager->isPendingMusicDelayed = false;
            manager->pendingMusicDelayTimer = 0.0f;
            manager->pendingMusicPath[0] = '\0'; // Clear pending music
        }
    }

    // Update music stream
    if (manager->isMusicPlaying && manager->music_count > 0) {
        UpdateMusicStream(manager->music[0].music);
    }
}

// Play sound effects
bool GamePlayMusic(AudioManager* manager, const char* filePath, float volume, bool loop, float fadeInDuration) {
    if (manager == NULL || filePath == NULL) return false;
    if (!manager->isMusicEnabled) return false;

    // If music is already playing, handle fade out
    if (manager->isMusicPlaying) {
        // Set pending music
        strncpy(manager->pendingMusicPath, filePath, sizeof(manager->pendingMusicPath) - 1);
        manager->pendingMusicVolume = volume;
        manager->pendingMusicLoop = loop;
        if (fadeInDuration > 0.0f) {
            manager->isFadingOut = true;
            manager->fadeOutDuration = fadeInDuration;
            manager->fadeOutTimer = 0.0f;
            manager->originalVolume = 1.0f; // Default volume, raylib's GetMusicVolume doesn't exist
        } else {
            StopMusicStream(manager->music[0].music);
            manager->isMusicPlaying = false;
            GamePlayMusic(manager, filePath, volume, loop, 0.0f);
        }
        return true;
    } else {
        // Load and play new music
        if (manager->music_count >= MAX_MUSIC_TRACKS) return false; 
        Music music = LoadMusicStream(filePath);
        if (music.ctxData == NULL) return false;
        manager->music[manager->music_count].type = MUSIC;
        manager->music[manager->music_count].music = music;
        manager->music_count++;
        SetMusicVolume(music, volume * manager->musicVolume * manager->masterVolume);
        PlayMusicStream(music);
        manager->isMusicPlaying = true;
        return true;
    }
}

// Play sound effects
bool GamePlaySound(AudioManager* manager, const char* filePath, AudioType type, float volume) {
    if (manager == NULL || filePath == NULL) return false;

    if (type == MUSIC) {
        if (!manager->isMusicEnabled) return false;
        if (manager->music_count >= MAX_MUSIC_TRACKS) {
            printf("Error: Maximum music track limit reached.\n");
            return false;
        }
        Music music = LoadMusicStream(filePath);
        if (music.ctxData == NULL) {
            printf("Error: Failed to load music from %s\n", filePath);
            return false;
        }
        manager->music[manager->music_count].type = MUSIC;
        manager->music[manager->music_count].music = music;
        manager->music_count++;
        SetMusicVolume(music, volume * manager->musicVolume * manager->masterVolume);
        PlayMusicStream(music);
        manager->isMusicPlaying = true;
        return true;
    } else {
        if (!manager->isSfxEnabled && type == SFX) return false;
        if (!manager->isVoxEnabled && type == VOX) return false;
        if (!manager->isAmbienceEnabled && type == AMBIENCE) return false;
        if (manager->sound_count >= MAX_SOUNDS) {
            printf("Error: Maximum sound effect limit reached.\n");
            return false;
        }
        Sound sound = LoadSound(filePath);
        if (sound.frameCount == 0) {
            printf("Error: Failed to load sound from %s\n", filePath);
            return false;
        }
        manager->sounds[manager->sound_count].type = type;
        manager->sounds[manager->sound_count].sound = sound;
        manager->sound_count++;
        float finalVolume = volume * manager->sfxVolume * manager->voxVolume * manager->ambienceVolume * manager->masterVolume;
        SetSoundVolume(sound, finalVolume);
        PlaySound(sound); // This is raylib's PlaySound
        return true;
    }
}

// Fade out current music
void FadeOutMusic(AudioManager* manager, float duration) {
    if (manager == NULL || !manager->isMusicPlaying) return;
    manager->isFadingOut = true;
    manager->fadeOutDuration = duration;
    manager->fadeOutTimer = 0.0f;
    manager->originalVolume = 1.0f; // Default volume, raylib's GetMusicVolume doesn't exist
}

// Set master volume
void GameSetMasterVolume(AudioManager* manager, float volume) {
    if (manager == NULL) return;
    manager->masterVolume = volume;
    if (manager->isMusicPlaying && manager->music_count > 0) {
        SetMusicVolume(manager->music[0].music, manager->musicVolume * manager->masterVolume);
    }
}

// Set music volume
void GameSetMusicVolume(AudioManager* manager, float volume) {
    if (manager == NULL) return;

    manager->musicVolume = volume;
    if (manager->isMusicPlaying && manager->music_count > 0) {
        SetMusicVolume(manager->music[0].music, volume * manager->masterVolume);
    }
}

// Set SFX volume
void SetSFXVolume(AudioManager* manager, float volume) {
    if (manager == NULL) return;
    manager->sfxVolume = volume;
    for (size_t i = 0; i < manager->sound_count; i++) {
        if (manager->sounds[i].type == SFX) {
            SetSoundVolume(manager->sounds[i].sound, volume * manager->masterVolume);
        }
    }
}

// Set VOX volume
void SetVOXVolume(AudioManager* manager, float volume) {
    if (manager == NULL) return;
    manager->voxVolume = volume;
    for (size_t i = 0; i < manager->sound_count; i++)
    {
        if (manager->sounds[i].type == VOX) {
            SetSoundVolume(manager->sounds[i].sound, volume * manager->masterVolume);
        }
    }
}

// Set Ambience volume
void SetAmbienceVolume(AudioManager* manager, float volume) {
    if (manager == NULL) return;
    manager->ambienceVolume = volume;
    for (size_t i = 0; i < manager->sound_count; i++) {
        if (manager->sounds[i].type == AMBIENCE) {
            SetSoundVolume(manager->sounds[i].sound, volume * manager->masterVolume);
        }
    }
}

// Stop current music
void StopMusic(AudioManager* manager) {
    if (manager == NULL || !manager->isMusicPlaying) return;
    StopMusicStream(manager->music[0].music);
    manager->isMusicPlaying = false;
}

// Stop all sound effects
void StopAllSFX(AudioManager* manager) {
    if (manager == NULL) return;
    for (size_t i = 0; i < manager->sound_count; i++) {
        if (manager->sounds[i].type == SFX || manager->sounds[i].type == VOX || manager->sounds[i].type == AMBIENCE) {
            StopSound(manager->sounds[i].sound);
        }
    }
}

// Get current volumes
float GameGetMasterVolume(AudioManager* manager) {
    return manager ? manager->masterVolume : 0.0f;
}

float GetMusicVolume(AudioManager* manager) {
    return manager ? manager->musicVolume : 0.0f;
}

float GetSFXVolume(AudioManager* manager) {
    return manager ? manager->sfxVolume : 0.0f;
}

float GetVOXVolume(AudioManager* manager) {
    return manager ? manager->voxVolume : 0.0f;
}  

float GetAmbienceVolume(AudioManager* manager) {
    return manager ? manager->ambienceVolume : 0.0f;
}

// Are various audio types enabled
bool IsSFXEnabled(AudioManager* manager) {
    return manager ? manager->isSfxEnabled : false;
}

bool IsVOXEnabled(AudioManager* manager) {
    return manager ? manager->isVoxEnabled : false;
}

bool IsAmbienceEnabled(AudioManager* manager) {
    return manager ? manager->isAmbienceEnabled : false;
}

bool IsMusicEnabled(AudioManager* manager) {
    return manager ? manager->isMusicEnabled : false;
}

// Is music currently playing
bool IsMusicPlayingNow(AudioManager* manager) {
    return manager ? manager->isMusicPlaying : false;
}
