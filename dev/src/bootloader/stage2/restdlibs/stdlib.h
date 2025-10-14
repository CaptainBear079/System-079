#pragma once

#include <stdint.h>
#include "./stdio.h"
#include "./../utils.h"

typedef struct __MEM_MAP_ENTRY__ {
    uint8_t Type;
    uint_t Size;
    uint_t Start;
    uint_t End;
} _MEM_MAP_ENTRY_;

typedef struct __SYS_MEMORY_MAP__ {
    uint_t BiggestAddress;
    uint_t BigestFreeMemory;
    uint_t EntryCount;
    _MEM_MAP_ENTRY_* entries;
} _SYS_MEMORY_MAP_;

_SYS_MEMORY_MAP_ MemMap;

void* malloc(size_t __size);
