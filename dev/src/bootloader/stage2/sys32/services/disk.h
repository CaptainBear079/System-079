//
// System 079 Disk Services v1.0.0
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

#ifndef __SYS__SERVICES_DISK_H_
#define __SYS__SERVICES_DISK_H_

#include <stdarg.h>
#include "./../../asm.h"
#include "./../../utils.h"
#include "./../../restdlibs/stdio.h"
#include "./../../restdlibs/stdlib.h"
#include "./../../restdlibs/string.h"
#include "floppy.h"

typedef struct _CHS_DiskGeometry_ {
    uint16_t HeadsPerCylinder;
    uint16_t SectorsPerTrack;
} CHS_DiskGeometry;

typedef struct _BIOS_DiskInfo_ {
    uint16_t Drive;
} BIOS_DiskInfo;

typedef struct _DiskHandler_ {
    BIOS_DiskInfo* BIOS__DiskHandler; // Disk information from BIOS
    CHS_DiskGeometry* CHS_Geometry;   // CHS disk geometry
    uint16_t Drive;                   // Drive containing the partition
    uint16_t PartitionNumber;         // Partition on the drive
    uint16_t DriveNameLength;         // Length of the drive name in DriveName
    char* DriveName;                  // Terminated by '/' or '\'
} DiskHandler;

typedef struct _CHAOS_DISK_GEOMETRY_ {
    uint16_t Drive;
    uint32_t PartitionNumber;

    uint16_t* SectorLength;
    uint32_t* ReservedSectors;
    uint16_t* RootEntries;
    uint32_t* DataTables;
    uint32_t* EntriesPerTable;

    uint32_t* ROOTDIR_POS;
    uint32_t* DATA_POS;

    uint8_t* path_Disk;
} CHAOS_DISK_GEOMETRY;

uint_t __SYS__SECTOR_SIZE;
extern bool __SYS__BIOS_COMPATIBLE;

DiskHandler InitDiskServices(const uint_t Drive, const char* DriveName, const uint_t PartitionNumber, const bool BIOS_Support, ...);
void* ReadSector_HDD(const bool UseDrivers, const DiskHandler* Handler, const uint_t StartLBA, const uint_t NumberOfSectorsToRead, const bool LoadToBuffer, const void* Buffer);

#endif
