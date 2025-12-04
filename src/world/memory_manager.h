// =============================================================
// Memory Manager Header
// =============================================================
// Simple memory management system for the framework
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>
#include <stdbool.h>

#define MAX_ALLOCATIONS 1024

typedef struct {
    void* ptr;
    size_t size;
    const char* file;
    int line;
    bool active;
} MemoryAllocation;

typedef struct {
    MemoryAllocation allocations[MAX_ALLOCATIONS];
    size_t totalAllocated;
    size_t totalFreed;
    int activeAllocations;
    bool initialized;
} MemoryManager;

// Function prototypes
bool InitMemoryManager(MemoryManager* manager);
void* ManagedAlloc(MemoryManager* manager, size_t size, const char* file, int line);
void* ManagedRealloc(MemoryManager* manager, void* ptr, size_t newSize, const char* file, int line);
void ManagedFree(MemoryManager* manager, void* ptr);
void UnloadMemoryManager(MemoryManager* manager);
void PrintMemoryStats(MemoryManager* manager);

// Convenience macros
#define MANAGED_ALLOC(manager, size) ManagedAlloc(manager, size, __FILE__, __LINE__)
#define MANAGED_REALLOC(manager, ptr, size) ManagedRealloc(manager, ptr, size, __FILE__, __LINE__)
#define MANAGED_FREE(manager, ptr) ManagedFree(manager, ptr)

#endif // MEMORY_MANAGER_H