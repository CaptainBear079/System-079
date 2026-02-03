//
// System 079 Bootloader Stage 2 ASM Header v1.0.0
// Date: 04 September 2025 Written by Captain Bear
// Email: hacklordcaptainbear@gmail.com
// GitHub: https://github.com/CaptainBear079/System-079
//
// By using, copying, or/and modifying this code, or the assembled machine code, you agree to the following:
// - You give credit to the original author(s)
// - You document all your changes
// - You don't remove or/and modify this notice
// - You add your own notice for your modifications and credit the changes author(s)
// - You agree to the extern license if there is one
// - You agree that the original author(s) do not hold any responsibility for your modifications
// - You are aware that this code is provided as-is without any warranty
// - You do not make money with it, when you want to make money with it, you have to ask for permission (email)
//      Note: Will be granted in most cases (as example: game development, most software, most cases of commercial use in operating systems), but you have to ask!
// - You do not make fun of this code or it's author(s), this also counts for the author(s) of changes and/or the users and community,
//      this includes any form of harassment or discrimination
// - You do not use this code or the assembled machine code for any illegal purposes
//
#pragma once
#include <stdint.h>
#include <stdbool.h>

extern uint16_t int13_GetParam_CX;
extern uint16_t int13_GetParam_DX;

extern uint32_t* MemoryMapBuffer;
extern uint32_t INT15hE820_EBX;

extern uint8_t __attribute__((cdecl)) asm_input(uint16_t port);
extern void __attribute__((cdecl)) asm_output(uint16_t port, uint8_t value);

// 16 Bit real mode
extern uint32_t __attribute__((cdecl)) asm_m16_DisplayText(char* text, uint32_t seg_offset, uint32_t column);

// BIOS Int 0x13 - Disk Services
extern bool __attribute__((cdecl)) asm_m16_int0x13_Reset(uint8_t drive);
extern void* __attribute__((cdecl)) asm_m16_int0x13_Read(uint8_t drive, uint16_t cylinder, uint8_t head, uint8_t sector, uint8_t num_sectors, void* buffer);
extern void __attribute__((cdecl)) asm_m16_int0x13_Get_Param(uint8_t drive);

// BIOS Int 0x13 Extensions - Disk Services
extern bool __attribute__((cdecl)) asm_m16_int0x13_Check_for_EDD(uint8_t drive);
extern void* __attribute__((cdecl)) asm_m16_int0x13_EDD_Read(uint8_t drive, uint16_t upper_upper_lba, uint16_t upper_lower_lba, uint16_t lower_upper_lba, uint16_t lower_lower_lba, uint8_t num_sectors, void* buffer);
extern void __attribute__((cdecl)) asm_m16_int0x13_EDD_Get_Param(uint8_t drive, void* buffer);

// BIOS Int 0x15 - Memory Services
extern int __attribute__((cdecl)) asm_m16_int0x15_E820h_Get_BIOS_Memory_Map(uint16_t MaxEntryCount, void* buffer);
