// Random utilities header
#pragma once 
#include <stdio.h>
#include <stdlib.h>
#include <random.h>
#include <stdint.h>

// Returns a random float between min and max
static inline float RandomFloatInRange(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

static inline int RandomIntInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

static inline uint8_t RandomByte() {
    return (uint8_t)(rand() % 256);
}

static inline uint8_t RandomByteInRange(uint8_t min, uint8_t max) {
    return (uint8_t)(min + rand() % (max - min + 1));
}