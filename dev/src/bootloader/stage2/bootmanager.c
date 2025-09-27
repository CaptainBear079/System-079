//
// System 079 Bootmanager (Bootloader Stage 2) v1.0.0
// Date: 04 September 2025 Written by Captain Bear
// Email: louis.ritz12@gmail.com
// GitHub: Currently private
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

// Set Library Mode to 32 Bit
#define __32BIT__

// Includes

#include "asm.h"
#include "./restdlibs/stdio.h" // Standard I/O (printf, ...) - WIP
#include "./sys32/services/disk.h" // Disk Services (FAT32, ChaosFormat, ...) - WIP

// Global Variables
// - System Info
uint16_t __SYS_BOOT_DRIVE = 0;
uint32_t __SYS_SCREEN_X = 0;
uint32_t __SYS_SCREEN_Y = 0;
uint32_t last_uint32_t_retcode = 0;

uint32_t __U32_0;
uint32_t __U32_1;
uint16_t __U16_0;
uint16_t __U16_1;
uint16_t __U16_2;
uint16_t __U16_3;

#define __KERNEL_LOAD_ADDRESS (void*)0x00000500 // Load Address for Kernels and Games
#define __SCREEN_BUFFER_AS_SEG_OFFSET ((__SYS_SCREEN_Y*80) + __SYS_SCREEN_X) * 2
#define __CONVERT_U64_TO_U16(u32_0, u32_1, u16_0, u16_1, u16_2, u16_3) \
    u16_0 = (uint16_t)((u32_0 >> 16) & 0xFFFF);  \
    u16_1 = (uint16_t)(u32_0 & 0xFFFF);          \
    u16_2 = (uint16_t)((u32_1 >> 16) & 0xFFFF);  \
    u16_3 = (uint16_t)(u32_1 & 0xFFFF);
#define R16_NEWLINE 0x0A

// Search for bootable kernels or games and load them for execution
int __attribute__((_cdecl)) _cstart(uint32_t boot_drive) {
    __SYS_BOOT_DRIVE = boot_drive; // Save the BIOS boot drive
    __U32_0 = 0;
    __U32_1 = 6684;
    __CONVERT_U64_TO_U16(__U32_0, __U32_1, __U16_0, __U16_1, __U16_2, __U16_3);

    clear_screen(); // Clear the screen
    extprintf(__SYS_SCREEN_X, __SYS_SCREEN_Y, "Hello, World!\nHello, World 2.0!\nHello, World 3.0!\nhex print test=%X\nuint32_t=%X && %X, uint16_t=%X && %X && %X && %X\n", (uint32_t)0x7465, (uint32_t)__U32_0, (uint32_t)__U32_1, (uint32_t)__U16_0, (uint32_t)__U16_1, (uint32_t)__U16_2, (uint32_t)__U16_3); // Test "printf" Hello World Meme by Captain Bear

    // Init Disk Services (FAT32, ChaosFormat, ...) - WIP
    //const DiskHandler Disk_ = InitDiskServices(__SYS_BOOT_DRIVE); // Init Root Disk (BootDrive)
    //ReadSector_HDD(&Disk_, 0, 1); // Test Read Sector from HDD (Read Sector 0 from BootDrive(Bootloader))

    // Jump to loaded kernel or game
    asm_m16_int0x13_EDD_Read(__SYS_BOOT_DRIVE, __U16_0, __U16_1, __U16_2, __U16_3, 1, __KERNEL_LOAD_ADDRESS);
    asm_jump_to_kernel(__KERNEL_LOAD_ADDRESS);

    // Bootmanager End
    return 0;
}