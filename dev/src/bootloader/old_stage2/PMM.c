#include "PMM.h"

// Print physical memory state
void pm__print_state() {
	// Print memory state
	char buffer[65];
	uint_t bitmap = 0;
	uint_t temp = 0;
	if(sizeof(uint_t) == sizeof(uint32_t)) {
		for(int i_i = 0; i_i < 2; i_i++) {
			bitmap = __MEMORY_BITMAP[i_i];
			for(int i = 0; i < 32; i++) {
				temp = (bitmap << i);
				temp = (temp & 0x80000000);
				if(temp == 0x80000000) {
					buffer[i + (32 * i_i)] = '1';
				}
				else {
					buffer[i + (32 * i_i)] = '0';
				}
			}
		}
	}
	else if(sizeof(uint_t) == sizeof(uint64_t)) {
		bitmap = __MEMORY_BITMAP[0];
		for(int i = 0; i < 64; i++) {
			temp = (bitmap << i);
			temp = (temp & 0x8000000000000000);
			if(temp == 0x80000000) {
				buffer[i] = '1';
			}
			else {
				buffer[i] = '0';
			}
		}
	}
	buffer[64] = '\0';
	printf(buffer);
	return;
}

// Search for chain of physical blocks of memory.
void* pm__search_memory(size_t* length) {
	// Find chain
	// Find free block
	uint_t bitmap = __MEMORY_BITMAP[0];
	uint_t i = 0;
	uint_t bitmap_section = 0;
	uint8_t bitmap_max_shift = 0;
	void* first_block = (void*)0;
	uint_t chain_length = 0;
	if(sizeof(uint_t) == sizeof(uint64_t)) {
		bitmap_max_shift = 63;
	}
	else {
		bitmap_max_shift = 31;
	}
	while(chain_length < length) {
		while((bitmap & 0x0001) != 1) {
			if(bitmap_section >= __MEMORY_BITMAP_SIZE) {
				goto PM__SEARCH_MEMORY__RET;
			}
			else if(i >= bitmap_max_shift) {
				i = 0;
				bitmap_section++;
				bitmap = __MEMORY_BITMAP[bitmap_section];
			}
			else {
				if(first_block != (void*)0) {
					void* current = (&__MEMORY_BITMAP[bitmap_section] + (bitmap_max_shift - i));
					for(uint64_t fb_i = (uint64_t)current - (uint64_t)first_block; fb_i > 0; fb_i--) {
						uint8_t* temp = (current - fb_i);
						*temp = (uint8_t)(*temp & 0x7F);
					}
					first_block = (void*)0;
				}
				i++;
				bitmap = bitmap >> i;
			}
		}
		goto PM__SEARCH_MEMORY__WHILE_END;
		PM__SEARCH_MEMORY__RET:
		return (void*)0;
		PM__SEARCH_MEMORY__WHILE_END:

		// Reserve block temporary
		__MEMORY_BITMAP[bitmap_section] = ((1 << i) & (bitmap << i));
		first_block = (&__MEMORY_BITMAP[bitmap_section] + ((1 << i) & (bitmap << i)));
	}

	// Free chain
	void* current = (&__MEMORY_BITMAP[bitmap_section] + (bitmap_max_shift - i));
	for(uint64_t fb_i = (uint64_t)current - (uint64_t)first_block; fb_i > 0; fb_i--) {
		uint8_t* temp = (current - fb_i);
		*temp = (uint8_t)(*temp & 0x7F);
	}

	// Return first block address
	return first_block;
}

// Allocate physical blocks of memory.
void* pm__malloc(size_t* blocks) {
    // Allocate blocks of memory (chained blocks)
	uint_t bitmap = __MEMORY_BITMAP[0];
	uint_t i = 0;
	uint_t bitmap_section = 0;
	uint8_t bitmap_max_shift = 0;
	void* first_block = (void*)0;
	uint_t chain_length = 0;
	if(sizeof(uint_t) == sizeof(uint64_t)) {
		bitmap_max_shift = 63;
	}
	else {
		bitmap_max_shift = 31;
	}
	while(chain_length < blocks) {
		while((bitmap & 0x0001) != 1) {
			if(bitmap_section >= __MEMORY_BITMAP_SIZE) {
				goto PM__SEARCH_MEMORY__RET;
			}
			else if(i >= bitmap_max_shift) {
				i = 0;
				bitmap_section++;
				bitmap = __MEMORY_BITMAP[bitmap_section];
			}
			else {
				if(first_block != (void*)0) {
					void* current = (&__MEMORY_BITMAP[bitmap_section] + (bitmap_max_shift - i));
					for(uint64_t fb_i = (uint64_t)current - (uint64_t)first_block; fb_i > 0; fb_i--) {
						uint8_t* temp = (current - fb_i);
						*temp = (uint8_t)(*temp & 0x7F);
					}
					first_block = (void*)0;
				}
				i++;
				bitmap = bitmap >> i;
			}
		}
		goto PM__SEARCH_MEMORY__WHILE_END;
		PM__SEARCH_MEMORY__RET:
		return (void*)0;
		PM__SEARCH_MEMORY__WHILE_END:

		// Reserve block temporary
		__MEMORY_BITMAP[bitmap_section] = ((1 << i) & (bitmap << i));
		first_block = (&__MEMORY_BITMAP[bitmap_section] + ((1 << i) & (bitmap << i)));
	}

	// Return first block address
	return first_block;
}

// Allocate a single block of memory.
void* pm__balloc() {
	// Find free block
	uint16_t bitmap = __MEMORY_BITMAP[0];
	uint16_t i = 0;
	uint16_t bitmap_section = 0;
	uint8_t bitmap_max_shift = 0;
	if(sizeof(uint_t) == sizeof(uint64_t)) {
		bitmap_max_shift = 63;
	}
	else {
		bitmap_max_shift = 31;
	}
	while((bitmap & 0x0001) != 1) {
		if(!(bitmap_section < __MEMORY_BITMAP_SIZE)) {
			goto PM__BALLOC__RET;
		}
		else if(i >= bitmap_max_shift) {
			i = 0;
			bitmap_section++;
			bitmap = __MEMORY_BITMAP[bitmap_section];
		}
		else {
			i++;
			bitmap = bitmap >> i;
		}
	}
	goto PM__BALLOC__WHILE_END;
	PM__BALLOC__RET:
	return (void*)0;
	PM__BALLOC__WHILE_END:

	// Reserve block
	__MEMORY_BITMAP[bitmap_section] = ((1 << i) & (bitmap << i));

	// Return address to block
	return (void*)(__MEMORY_BITMAP + ((bitmap_section * (bitmap_max_shift + 1)) + (bitmap_max_shift - i)));
}