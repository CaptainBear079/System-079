#include "other.h"

// Set LEN bytes in DEST to VAL
void* memset(void* __dest, int __val, uint16_t __len) {
    uint8_t* ptr = (uint8_t*)__dest;

    for(uint16_t i = 0; i < __len; i++) {
        ptr[i] = (uint8_t)__val;
    }

    return __dest;
}