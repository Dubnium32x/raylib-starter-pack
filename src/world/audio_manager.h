// =============================================================
// Audio Manager header
// =============================================================
// This header defines the AudioManager structure and related
// functions for managing audio playback, including music,
// sound effects, and module player integration for tracker music.
// =============================================================
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H
#include "raylib.h"
#include "../util/globals.h"

// Audio max counts (You can adjust these as needed)
#define MAX_SOUNDS 100
#define MAX_MUSIC_TRACKS 10

// Audio types
typedef enum {
    MUSIC,
    SFX,
    VOX,
    AMBIENCE
} AudioType;

// Audio resource structure
typedef struct {
    AudioType type;
    Sound sound;
    Music music;
} AudioResource;

// Audio Manager structure
typedef struct {
    AudioResource sounds[MAX_SOUNDS];
    AudioResource music[MAX_MUSIC_TRACKS];
    size_t sound_count;
    size_t music_count;
    float masterVolume;
    float musicVolume;
    float sfxVolume;
    float voxVolume;
    float ambienceVolume;

    // Audio enabled flags
    bool isMusicPlaying;
    bool isMusicEnabled;
    bool isSfxEnabled;
    bool isVoxEnabled;
    bool isAmbienceEnabled;

    // For music fading in/out
    bool isFadingOut;
    float fadeOutDuration;
    float fadeOutTimer;
    float originalVolume;
    char pendingMusicPath[256];
    float pendingMusicVolume;
    bool pendingMusicLoop;
    float pendingMusicDelay;
    float pendingMusicDelayTimer;
    bool isPendingMusicDelayed;
} AudioManager;

// Audio Manager function declarations
void InitAudioManager(AudioManager* manager);
void UpdateAudioManager(AudioManager* manager, float deltaTime);
bool GamePlaySound(AudioManager* manager, const char* filePath, AudioType type, float volume);
bool GamePlayMusic(AudioManager* manager, const char* filePath, float volume, bool loop, float fadeInDuration);
void FadeOutMusic(AudioManager* manager, float duration);
void GameSetMasterVolume(AudioManager* manager, float volume);
void GameSetMusicVolume(AudioManager* manager, float volume);
void SetSFXVolume(AudioManager* manager, float volume);
void SetVOXVolume(AudioManager* manager, float volume);
void SetAmbienceVolume(AudioManager* manager, float volume);
void StopMusic(AudioManager* manager);
void StopAllSFX(AudioManager* manager);

// Getter function declarations
float GameGetMasterVolume(AudioManager* manager);
float GetMusicVolume(AudioManager* manager);
float GetSFXVolume(AudioManager* manager);
float GetVOXVolume(AudioManager* manager);
float GetAmbienceVolume(AudioManager* manager);
bool IsMusicPlayingNow(AudioManager* manager);
bool IsMusicEnabled(AudioManager* manager);
bool IsSFXEnabled(AudioManager* manager);
bool IsVOXEnabled(AudioManager* manager);
bool IsAmbienceEnabled(AudioManager* manager);

#endif // AUDIO_MANAGER_H