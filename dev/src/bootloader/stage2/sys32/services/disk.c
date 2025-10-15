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

DiskHandler InitDiskServices(const uint_t Drive, const char* DriveName, const uint_t PartitionNumber, const bool BIOS_Support, ...) {
	va_list args;
	va_start(args, BIOS_Support);
	DiskHandler Handler;
	if(BIOS_Support == true) {
		Handler.BIOS__DiskHandler = va_arg(args, BIOS_DiskInfo*);
		Handler.BIOS__DiskHandler->Drive = va_arg(args, uint_t);
	}
	Handler.Drive = Drive;
	Handler.DriveNameLength = strlen(DriveName);
	Handler.DriveName = malloc(Handler.DriveNameLength * sizeof(char));
	Handler.PartitionNumber = PartitionNumber;
	va_end(args);
	return Handler;
}

void* ReadSector_HDD(const bool UseDrivers, const DiskHandler* Handler, const uint_t StartLBA, const uint_t NumberOfSectorsToRead, const bool LoadToBuffer, const void* Buffer) {
	void* buffer = NULL;
	// Check system state
	if(UseDrivers) {
		// Read from Disk using drivers
	}
	else {
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
			if(sizeof(uint_t) == sizeof(uint64_t)) {
				// 64 Bit LBA
				LBA_0 = (uint16_t)((StartLBA >> 48) & 0xFFFFFFFFFFFFFFFF);
				LBA_1 = (uint16_t)((StartLBA >> 32) & 0xFFFFFFFFFFFFFFFF);
				LBA_2 = (uint16_t)((StartLBA >> 16) & 0xFFFFFFFFFFFFFFFF);
				LBA_3 = (uint16_t)(StartLBA & 0xFFFFFFFFFFFFFFFF);
			}
			else {
				// 32 Bit LBA
				LBA_0 = (uint16_t)0x0000;
				LBA_1 = (uint16_t)0x0000;
				LBA_2 = (uint16_t)((StartLBA >> 16) & 0xFFFF);
				LBA_3 = (uint16_t)(StartLBA & 0xFFFF);
			}

			// Read with EDD
			if(LoadToBuffer) {
				buffer = asm_m16_int0x13_EDD_Read(Handler->BIOS__DiskHandler->Drive, LBA_0, LBA_1, LBA_2, LBA_3, NumberOfSectorsToRead, Buffer);
			}
			else {
				buffer = asm_m16_int0x13_EDD_Read(Handler->BIOS__DiskHandler->Drive, LBA_0, LBA_1, LBA_2, LBA_3, NumberOfSectorsToRead, malloc(NumberOfSectorsToRead * __SYS__SECTOR_SIZE));
			}
		}
		else {
			// Calculate CHS
			uint16_t cylinder;
			uint8_t head;
			uint8_t sector;
			convert_LBA_to_CHS(StartLBA, &cylinder, &head, &sector, Handler->CHS_Geometry);

			// Read with old INT 13h
			if(LoadToBuffer) {
				buffer = asm_m16_int0x13_Read(Handler->BIOS__DiskHandler->Drive, cylinder, head, sector, NumberOfSectorsToRead, Buffer);
			}
			else {
				buffer = asm_m16_int0x13_Read(Handler->BIOS__DiskHandler->Drive, cylinder, head, sector, NumberOfSectorsToRead, malloc(NumberOfSectorsToRead * __SYS__SECTOR_SIZE));
			}
		}
	}
	return buffer;
}