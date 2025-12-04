// =============================================================
// Memory Manager Implementation
// =============================================================
// Simple memory management system implementation

#include "memory_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool InitMemoryManager(MemoryManager* manager) {
    if (!manager) return false;
    
    memset(manager, 0, sizeof(MemoryManager));
    manager->totalAllocated = 0;
    manager->totalFreed = 0;
    manager->activeAllocations = 0;
    manager->initialized = true;
    
    return true;
}

void* ManagedAlloc(MemoryManager* manager, size_t size, const char* file, int line) {
    if (!manager || !manager->initialized) return malloc(size);
    
    void* ptr = malloc(size);
    if (!ptr) return NULL;
    
    // Find empty slot
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        if (!manager->allocations[i].active) {
            manager->allocations[i].ptr = ptr;
            manager->allocations[i].size = size;
            manager->allocations[i].file = file;
            manager->allocations[i].line = line;
            manager->allocations[i].active = true;
            manager->totalAllocated += size;
            manager->activeAllocations++;
            break;
        }
    }
    
    return ptr;
}

void* ManagedRealloc(MemoryManager* manager, void* ptr, size_t newSize, const char* file, int line) {
    if (!manager || !manager->initialized) return realloc(ptr, newSize);
    
    // Find existing allocation
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        if (manager->allocations[i].active && manager->allocations[i].ptr == ptr) {
            void* newPtr = realloc(ptr, newSize);
            if (newPtr) {
                manager->totalFreed += manager->allocations[i].size;
                manager->totalAllocated += newSize;
                manager->allocations[i].ptr = newPtr;
                manager->allocations[i].size = newSize;
                manager->allocations[i].file = file;
                manager->allocations[i].line = line;
            }
            return newPtr;
        }
    }
    
    // Not found in our tracking, just use regular realloc
    return realloc(ptr, newSize);
}

void ManagedFree(MemoryManager* manager, void* ptr) {
    if (!ptr) return;
    
    if (!manager || !manager->initialized) {
        free(ptr);
        return;
    }
    
    // Find allocation
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        if (manager->allocations[i].active && manager->allocations[i].ptr == ptr) {
            manager->totalFreed += manager->allocations[i].size;
            manager->allocations[i].active = false;
            manager->activeAllocations--;
            break;
        }
    }
    
    free(ptr);
}

void UnloadMemoryManager(MemoryManager* manager) {
    if (!manager || !manager->initialized) return;
    
    // Free any remaining allocations
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        if (manager->allocations[i].active) {
            free(manager->allocations[i].ptr);
            manager->allocations[i].active = false;
        }
    }
    
    manager->initialized = false;
}

void PrintMemoryStats(MemoryManager* manager) {
    if (!manager || !manager->initialized) return;
    
    printf("=== Memory Manager Stats ===\n");
    printf("Total Allocated: %zu bytes\n", manager->totalAllocated);
    printf("Total Freed: %zu bytes\n", manager->totalFreed);
    printf("Active Allocations: %d\n", manager->activeAllocations);
    printf("Memory Leaked: %zu bytes\n", manager->totalAllocated - manager->totalFreed);
    
    if (manager->activeAllocations > 0) {
        printf("\nActive allocations:\n");
        for (int i = 0; i < MAX_ALLOCATIONS; i++) {
            if (manager->allocations[i].active) {
                printf("  %p - %zu bytes - %s:%d\n", 
                       manager->allocations[i].ptr,
                       manager->allocations[i].size,
                       manager->allocations[i].file,
                       manager->allocations[i].line);
            }
        }
    }
}