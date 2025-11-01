//
// System 079 Disk Services v1.0.0
// Date: 04 September 2025 Written by Captain Bear
// Email: louis.ritz12@gmail.com
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

#include "disk.h"

void* ReadSector_HDD(const DiskHandler* Handler, const uint_t StartLBA, const uint_t NumberOfSectorsToRead, const void* Buffer) {
	void* buffer = NULL;
	// Check if BIOS compatible
	if(Handler->BIOS__DiskHandler == NULL && __SYS__BIOS_COMPATIBLE) {
		return NULL;
	}
	// Read Sector from HDD using BIOS Int 0x13
	if(asm_m16_int0x13_Check_for_EDD(Handler->BIOS__DiskHandler->Drive)) {
		// Calculate LBA parts for EDD
		uint16_t LBA_0;
		uint16_t LBA_1;
		uint16_t LBA_2;
		uint16_t LBA_3;
		// 32 Bit LBA
		LBA_0 = (uint16_t)0x0000;
		LBA_1 = (uint16_t)0x0000;
		LBA_2 = (uint16_t)((StartLBA >> 16) & 0xFFFF);
		LBA_3 = (uint16_t)(StartLBA & 0xFFFF);
		// Read with EDD
		buffer = asm_m16_int0x13_EDD_Read(Handler->BIOS__DiskHandler->Drive, LBA_0, LBA_1, LBA_2, LBA_3, NumberOfSectorsToRead, (void*)0x0500);
	}
	else {
		// Calculate CHS
		uint16_t cylinder;
		uint8_t head;
		uint8_t sector;
		convert_LBA_to_CHS(StartLBA, &cylinder, &head, &sector, Handler->CHS_Geometry);
		// Read with old INT 13h
		buffer = asm_m16_int0x13_Read(Handler->BIOS__DiskHandler->Drive, cylinder, head, sector, NumberOfSectorsToRead, (void*)0x0500);
	}
	return buffer;
}