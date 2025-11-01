// Physical Memory Manager (PMM)
#define __32BIT__
// Includes
#include <stdint.h>
#include "./../../restdlibs/stdio.h"

// Defines
#if defined(__32BIT__)
    typedef uint32_t uint_t;
    typedef uint_t size_t;
#else
    typedef uint64_t uint_t;
    typedef uint_t size_t;
#endif

// - Memory bit map
extern uint16_t __MEMORY_BITMAP_SIZE;
extern uint16_t* __MEMORY_BITMAP;

// Print physical memory state
void pm__print_state();

// Search for chain of physical blocks of memory.
void* pm__search_memory(size_t* length);

// Allocate physical blocks of memory
void* pm__malloc(size_t* blocks);

// Allocate a single block of memory
void* pm__balloc();