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