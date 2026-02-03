//
// System 079 Bootmanager UTILS v1.0.0
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
#include "utils.h"

// Coverting CHS to LBA
int convert_CHS_to_LBA(const uint16_t cylinder, const uint8_t head, const uint8_t sector, uint_t* LBA, CHS_DiskGeometry* CHS_Geo) {
    *LBA = (cylinder * CHS_Geo->HeadsPerCylinder + head) * CHS_Geo->SectorsPerTrack + (sector - 1);
    return 0;
}

// Converting LBA to CHS
int convert_LBA_to_CHS(const uint_t LBA, uint16_t* cylinder, uint8_t* head, uint8_t* sector, CHS_DiskGeometry* CHS_Geo) {
    *cylinder = LBA / (CHS_Geo->HeadsPerCylinder * CHS_Geo->SectorsPerTrack);
    unsigned long temp = LBA % (CHS_Geo->HeadsPerCylinder * CHS_Geo->SectorsPerTrack);
    *head = temp / CHS_Geo->SectorsPerTrack;
    *sector = (temp % CHS_Geo->SectorsPerTrack) + 1;
    return 0;
}