#pragma once

#include <stdint.h>

// CHS structure
typedef struct {
    uint16_t cylinder;
    uint8_t head;
    uint8_t sector;
} CHS;

// LBA structure
typedef struct {
    uint32_t lba;
} LBA;

LBA CHS_TO_LBA(CHS chs);
CHS LBA_TO_CHS(LBA lba);