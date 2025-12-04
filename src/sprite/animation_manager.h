// =============================================================
// Animation Manager header
// =============================================================
// This header defines the AnimationManager structure and related
#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include "raylib.h"
#include "../util/globals.h"

// Animation manager constants
#define MAX_ANIMATIONS 100
#define MAX_ANIMATION_NAME_LENGTH 64

// Animation sequence types
typedef enum {
    ANIMATION_SEQUENCE_LOOP,
    ANIMATION_SEQUENCE_ONCE,
    ANIMATION_SEQUENCE_PINGPONG
} AnimationSequenceType;

// Animation frame structure
typedef struct {
    int frameIndex;
    float duration;
} AnimationFrame;

// Animation sequence structure
typedef struct {
    const char* name;
    AnimationSequenceType sequenceType;
    int frameCount;
    AnimationFrame* frames;
} AnimationSequence;

// Animator structure
typedef struct {
    AnimationSequence sequence;
    AnimationFrame *currentFrame;
    AnimationSequenceType currentType;
    int currentFrameIndex;
    float frameTimer;
    float speedMultiplier;
} Animator;

// Animation manager state enum
typedef enum {
    ANIMATION_MANAGER_UNINITIALIZED,
    ANIMATION_MANAGER_INITIALIZED
} AnimationManagerState;

// Animation manager structure
typedef struct {
    Animator* animators[MAX_ANIMATIONS];
    size_t animatorCount;
    AnimationManagerState state;
    float frameTime;
    float elapsedTime;
    const char* animations;
    Texture2D texture;
} AnimationManager;

// Animation manager functions
void InitAnimationManager(AnimationManager* manager);
void AddAnimator(AnimationManager* manager, Animator* animator);
void RemoveAnimator(AnimationManager* manager, const char* name);
void UpdateAnimators(AnimationManager* manager, float deltaTime);
void PlayAnimation(Animator* animator, const char* animationName);
void StopAnimation(Animator* animator);
void SetAnimationSpeed(Animator* animator, float speedMultiplier);
bool IsAnimationPlaying(const Animator* animator);
void UnloadAllAnimators(AnimationManager* manager);
void SetAnimationState(Animator* animator, AnimationSequenceType state);
AnimationSequence* GetAnimationByName(const char* name);
// Return the source rectangle for a given frame index in the current texture/spritesheet
Rectangle GetAnimationFrameRect(const AnimationManager* manager, int frameIndex);
#endif // ANIMATION_MANAGER_H
