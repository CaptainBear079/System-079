// Physical Memory Manager (PMM)
// Includes
#include <stdint.h>

// - Memory bit map
extern uint16_t* __MEMORY_BITMAP;

// Search for chain of physical blocks of memory.
void* pm__search_memory(uint8_t arg_length_size, void* length);

// Allocate physical blocks of memory
void* pm__malloc(uint8_t arg_blocks_size, void* blocks);

// Allocate a single block of memory
void* pm__balloc();