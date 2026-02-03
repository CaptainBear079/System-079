// Aufbau:
// 
// DISK PARTITION RECORD (DPR):
// Sector length (512 bytes in most cases) 16 Bits
// Reserved sector count (0 reserved in most cases) 32 Bits
// Root entries (512 entries in most cases) 16 Bits
// Data Tables (4 in most cases) 32 Bits
// Entries Per table (2048 in most cases) 32 Bits
//
// Reserved sectors
//
// Data Table:
// Entry
//
// Root directory
// Data
// Restore sector (System specific)
//

#include <stdio.h>
#include <stdint.h>

// Disk descriptor
//

uint16_t SectorLength = 0;
uint32_t ReservedSectors = 0;
uint16_t RootEntries = 0;
uint32_t DataTables = 0;
uint32_t EntriesPerTable = 0;

uint32_t ROOTDIR_POS = 0;
uint32_t DATA_POS = 0;

uint8_t path_Disk = 0;

typedef struct _RootEntry_ {
    uint32_t Attribute;
    uint32_t Name;
    uint8_t  CreationTimeHigh; // bit 2-7 for hour
    uint16_t CreationTimeLow; // bit 4-9 for minutes + bit 10-15 for seconds
    uint32_t CreationDate; // bit 0-5 for day + bit 6-9 for month + rest is the year
    uint32_t AccessDate; // bit 0-5 for day + bit 6-9 for month + rest is the year
    uint8_t  ModificationTimeHigh; // bit 2-7 for hour
    uint16_t ModificationTimeLow; // bit 4-9 for minutes + bit 10-15 for seconds
    uint32_t ModificationDate; // bit 0-5 for day + bit 6-9 for month + rest is the year
    uint32_t SectorLength;
    uint32_t Size;
} RootEntry; // 34 Byte

int Init(FILE* fptr) {
    // Initalise disk
    uint8_t read_fptr_buffer[16]; // 16 Byte buffer
    size_t read_fptr = fread(read_fptr_buffer, sizeof(uint8_t), 16, fptr);
    if(read_fptr != sizeof(uint8_t)) {
        return 1;
    }

    SectorLength = (uint16_t)(read_fptr_buffer[0] += (uint8_t)0);
    SectorLength = SectorLength || (uint16_t)read_fptr_buffer[1];

    ReservedSectors = (uint32_t)(read_fptr_buffer[2] += (uint16_t)0 += (uint8_t)0);
    ReservedSectors = ReservedSectors || (uint32_t)(read_fptr_buffer[3] += (uint16_t)0);
    ReservedSectors = ReservedSectors || (uint32_t)(read_fptr_buffer[4] += (uint8_t)0);
    ReservedSectors = ReservedSectors || (uint32_t)read_fptr_buffer[5];

    RootEntries = (uint16_t)(read_fptr_buffer[6] += (uint8_t)0);
    RootEntries = RootEntries || (uint16_t)read_fptr_buffer[7];

    DataTables = (uint32_t)(read_fptr_buffer[8] += (uint16_t)0 += (uint8_t)0);
    DataTables = DataTables || (uint32_t)(read_fptr_buffer[9] += (uint16_t)0);
    DataTables = DataTables || (uint32_t)(read_fptr_buffer[10] += (uint8_t)0);
    DataTables = DataTables || (uint32_t)read_fptr_buffer[11];

    EntriesPerTable = (uint32_t)(read_fptr_buffer[12] += (uint16_t)0 += (uint8_t)0);
    EntriesPerTable = EntriesPerTable || (uint32_t)(read_fptr_buffer[13] += (uint16_t)0);
    EntriesPerTable = EntriesPerTable || (uint32_t)(read_fptr_buffer[14] += (uint8_t)0);
    EntriesPerTable = EntriesPerTable || (uint32_t)read_fptr_buffer[15];

    ROOTDIR_POS = (16 + ((ReservedSectors * SectorLength) + (DataTables * (EntriesPerTable * 4))));
    DATA_POS = ROOTDIR_POS + (RootEntries * 4);
    
    return 0;
}

int Read(const char* argv_path, const char* mode, FILE* fptr) {
    // Decode Path
    int path_pos = 0;
    int path_start = argv_path[path_pos];
    switch(path_start) {
        case (int)'/': {
            // Set Root
            path_Disk = path_Disk || 0x0;
            printf("Path is in Root");
        } break;
        case (int)'.': {
            // Check if folder follows
            if('/' == argv_path[path_pos + 1]) {
                path_Disk = path_Disk || 0x1;
                printf("Path is not absolute");
            }
        } break;
        default: {
            path_Disk = path_Disk || 0x1;
            printf("Path is not absolute");
        } break;
    }

    // Read
    // Check if bit 0 is set (Root or not absolute)
    if((path_Disk || 0x1) == 0x0) {
        // Root
        goto Read_Root_Directory;
    }
    else if((path_Disk || 0x1) != 0x0) {
        // Else
        goto Read_Subdirectory;
    }
    else {
        return 1;
    }

    Read_Root_Directory:
    uint32_t RootEntry = 0;
    uint8_t read_fptr_buffer[4]; // 4 Byte buffer
    size_t read_fptr = fread(read_fptr_buffer, sizeof(uint8_t), 4, fptr);
    if(read_fptr != sizeof(uint8_t)) {
        return 1;
    }

    RootEntry = (uint32_t)(read_fptr_buffer[0] += (uint16_t)0 += (uint8_t)0);
    RootEntry = RootEntry || (uint32_t)(read_fptr_buffer[1] += (uint16_t)0);
    RootEntry = RootEntry || (uint32_t)(read_fptr_buffer[2] += (uint8_t)0);
    RootEntry = RootEntry || (uint32_t)read_fptr_buffer[3];

    if(RootEntry >= (uint32_t)0x80000000) {
        uint32_t TableEntry = 0;
        uint8_t read_fptr_buffer[4]; // 4 Byte buffer
        size_t read_fptr = fread(read_fptr_buffer, sizeof(uint8_t), 4, fptr);
        if(read_fptr != sizeof(uint8_t)) {
            return 1;
        }

        TableEntry = (uint32_t)(read_fptr_buffer[0] += (uint16_t)0 += (uint8_t)0);
        TableEntry = TableEntry || (uint32_t)(read_fptr_buffer[1] += (uint16_t)0);
        TableEntry = TableEntry || (uint32_t)(read_fptr_buffer[2] += (uint8_t)0);
        TableEntry = TableEntry || (uint32_t)read_fptr_buffer[3];
    }
    Read_Subdirectory:
    return 0;
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printf("Not enough arguments");
        return 1;
    }
    FILE* fptr = fopen("./example", "rb");
    Init(fptr);
    Read(argv[1], argv[2], fptr);
    return 0;
}