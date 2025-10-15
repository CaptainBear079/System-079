#pragma once

#include "./stdio.h"
#include "./../utils.h"

extern _SYS_MEMORY_MAP_ MemMap;
extern uint32_t INT15hE820_EBX;

// Get Bootloader Memory Map. DO NOT USE THIS FUNCTION AFTER OVERWRITING BIOS DATA!
int __SYS_Init_Memory_Map(const bool UseBIOS);

// Allocate SIZE bytes of memory.
void* malloc(size_t __size);
