#include "PMM.h"

// Search for chain of physical blocks of memory.
void* pm__search_memory(uint8_t arg_length_size, void* length) {}

// Allocate physical blocks of memory.
void* pm__malloc(uint8_t arg_blocks_size, void* blocks) {
    // Allocate blocks of memory (chained blocks)
}

// Allocate a single block of memory.
void* pm__balloc() {
	// Find free block
	uint16_t bitmap = __MEMORY_BITMAP[0];
	uint16_t i = 0;
	uint16_t bitmap_section = 0;
	while((bitmap & 0x0001) != 1) {
		if(i >= 15) {
			i = 0;
			bitmap_section++;
			bitmap = __MEMORY_BITMAP[bitmap_section];
		}
		else {
			i++;
			bitmap = bitmap >> i;
		}
	}

	// Reserve block
	__MEMORY_BITMAP[bitmap_section] = ((1 << i) & (bitmap << i));

	// Return address to block
	return (void*)(__MEMORY_BITMAP + ((bitmap_section * 16) + (16 - i)));
}