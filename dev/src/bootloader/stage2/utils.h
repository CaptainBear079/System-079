#pragma once

#include <stdint.h>
#include "./sys32/services/disk.h"

#if defined(__32BIT__)
    #define uint_t uint32_t
    #define size_t uint_t
#else
    #define uint_t uint64_t
    #define size_t uint_t
#endif

// Coverting CHS to LBA
int convert_CHS_to_LBA(const uint16_t cylinder, const uint8_t head, const uint8_t sector, uint_t* LBA, CHS_DiskGeometry* CHS_Geo);
// Converting LBA to CHS
int convert_LBA_to_CHS(const uint_t LBA, uint16_t* cylinder, uint8_t* head, uint8_t* sector, CHS_DiskGeometry* CHS_Geo);
