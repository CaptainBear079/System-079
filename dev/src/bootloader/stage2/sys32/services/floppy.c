#include "floppy.h"

#define FLOPPY_SECTORS_PER_TRACK 18
#define FLOPPY_HEADS 2

// CHS to LBA
LBA CHS_TO_LBA(CHS chs) {
    LBA lba;
    lba.lba = (chs.cylinder * FLOPPY_HEADS + chs.head) * FLOPPY_SECTORS_PER_TRACK + (chs.sector - 1);
    return lba;
}

// LBA to CHS
CHS LBA_TO_CHS(LBA lba) {
    CHS chs;
    chs.cylinder = lba.lba / (FLOPPY_HEADS * FLOPPY_SECTORS_PER_TRACK);
    chs.head = (lba.lba / FLOPPY_SECTORS_PER_TRACK) % FLOPPY_HEADS;
    chs.sector = (lba.lba % FLOPPY_SECTORS_PER_TRACK) + 1;
    return chs;
}
