//
// System 079 Bootmanager (Bootloader Stage 2) v1.0.0
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

// Set Library Mode to 32 Bit
#define __32BIT__

// Includes

#include "utils.h"
#include "asm.h"
#include "stdio.h"     // Standard I/O (printf, ...) - WIP
#include "disk.h" // Disk Services (FAT32, ChaosFormat, ...) - WIP

// Global Variables
// - System Info
uint16_t __SYS_BOOT_DRIVE = 0;
uint32_t __SYS_SCREEN_X = 0;
uint32_t __SYS_SCREEN_Y = 0;
uint_t __SYS__SECTOR_SIZE = 512;
bool __SYS__BIOS_COMPATIBLE = true;
_SYS_MEMORY_MAP_ MemMap;
uint8_t* ScreenBuffer = (uint8_t*)0xB8000;

#define __KERNEL_TARGET 0x00100000
#define __SCREEN_BUFFER_AS_SEG_OFFSET ((__SYS_SCREEN_Y*80) + __SYS_SCREEN_X) * 2

typedef void (*KernelStart)();

// Memcpy function
void* memcpy(void* __dest, const void* __src, uint16_t __len) {
	uint8_t* d = (uint8_t*)__dest;
	const uint8_t* s = (const uint8_t*)__src;
	for(uint16_t i = 0; i < __len; i++) {
		d[i] = s[i];
	}
	return __dest;
}

// Search for bootable kernels or games and load them for execution
int __attribute__((_cdecl)) _cstart(uint32_t boot_drive) {
	__SYS_BOOT_DRIVE = boot_drive; // Save the BIOS boot drive

	// Set system data since there is no normal c setup for global variables
	__SYS_SCREEN_X = 0;
	__SYS_SCREEN_Y = 0;
	__SYS__SECTOR_SIZE = 512;
	__SYS__BIOS_COMPATIBLE = true;

	clear_screen(); // Clear the screen
	set_pos(__SYS_SCREEN_X, __SYS_SCREEN_Y); // Set writing position

	// Init Disk Services (FAT32, ChaosFormat, ...) - WIP
	BIOS_DiskInfo biosDisk;
	DiskHandler Disk_;
	Disk_.BIOS__DiskHandler = &biosDisk;
	Disk_.BIOS__DiskHandler->Drive = __SYS_BOOT_DRIVE;
	Disk_.Drive = 0;
	Disk_.DriveNameLength = 1;
	Disk_.DriveName[0] = '/';
	Disk_.DriveName[1] = '\0';
	Disk_.PartitionNumber = 0;
	void* __KERNEL_LOAD_ADDRESS = (void*)0x00000500;
	for(int i = 0; i < 10; i++) {
		__KERNEL_LOAD_ADDRESS = ReadSector_HDD(&Disk_, (6684 + i), 1, __KERNEL_LOAD_ADDRESS);
		if(__KERNEL_LOAD_ADDRESS == NULL) {
			printf("[ERROR] Failed to read kernel from disk. Exit code: 0x0001");
			return 0x0001;
		}
		else {
			memcpy((void*)(__KERNEL_TARGET + (i * 512)), __KERNEL_LOAD_ADDRESS, 512); // Copy to correct location
			for(int c = 0; c < 512; c++) {
				if(*((uint8_t*)(__KERNEL_TARGET + (c + (i * 512)))) != *((uint8_t*)(__KERNEL_LOAD_ADDRESS + c))) {
					printf("[ERROR] Kernel load error. Exit code: 0x0002");
					return 0x0002;
				}
				c += 8;
			}
		}
	}
	KernelStart kernel = (void*)0x00100000;
	kernel();

	// Bootmanager End
	return 0;
}