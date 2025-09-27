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

#include "disk.h"

DiskHandler InitDiskServices(uint16_t boot_drive) {
    DiskHandler handler;
    handler.drive = boot_drive;
    return handler;
}

void ReadSector_HDD(const DiskHandler* handler, uint32_t FirstSectorToRead, uint8_t NumberOfSectorsToRead) {
    // Read Sector from HDD using BIOS Int 0x13
}