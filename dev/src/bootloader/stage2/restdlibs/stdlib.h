#pragma once

#include "./stdio.h"
#include "./../utils.h"

extern _SYS_MEMORY_MAP_ MemMap;

// Allocate SIZE bytes of memory.
void* malloc(size_t __size);
