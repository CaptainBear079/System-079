#pragma once
#include <stdint.h>

extern uint16_t int13_GetParam_CX;
extern uint16_t int13_GetParam_DX;

extern uint8_t __attribute__((cdecl)) asm_input(uint16_t port);
extern void __attribute__((cdecl)) asm_output(uint16_t port, uint8_t value);

// 16 Bit real mode
extern uint32_t __attribute__((cdecl)) asm_m16_DisplayText(char* text, uint32_t seg_offset, uint32_t column);

// BIOS Int 0x13 - Disk Services
extern uint32_t __attribute__((cdecl)) asm_m16_int0x13_Reset(uint8_t drive);
extern void __attribute__((cdecl)) asm_m16_int0x13_Read(uint8_t drive, uint16_t cylinder, uint8_t head, uint8_t sector, uint8_t num_sectors, void* buffer);
extern void __attribute__((cdecl)) asm_m16_int0x13_Get_Param(uint8_t drive);

// BIOS Int 0x13 Extensions - Disk Services
extern void __attribute__((cdecl)) asm_m16_int0x13_EDD_Read(uint8_t drive, uint16_t upper_upper_lba, uint16_t upper_lower_lba, uint16_t lower_upper_lba, uint16_t lower_lower_lba, uint8_t num_sectors, void* buffer);
extern void __attribute__((cdecl)) asm_m16_int0x13_EDD_Get_Param(uint8_t drive, void* buffer);

extern void __attribute__((cdecl)) asm_jump_to_kernel(void* address);