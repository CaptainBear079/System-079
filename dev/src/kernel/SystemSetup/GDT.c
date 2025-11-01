#include "GDT.h"

GDT_ENTRY GDT_Entries[] = {
    // Null entry
    GDT_ENTRY_M( 0, 0, 0, 0 ),
    // 16 Bit Code segment
    GDT_ENTRY_M( 0,
        0xFFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE | GDT_ACCESS_CODE_READABLE,
        GDT_FLAGS_16BIT | GDT_FLAGS_GRANULARITY_1BYTE),
    // 16 Bit Data segment
    GDT_ENTRY_M( 0,
        0xFFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA | GDT_ACCESS_DATA_WRITABLE,
        GDT_FLAGS_16BIT | GDT_FLAGS_GRANULARITY_1BYTE),
    // 32 Bit Code segment
    GDT_ENTRY_M( 0,
        0xFFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE | GDT_ACCESS_CODE_READABLE,
        GDT_FLAGS_32BIT | GDT_FLAGS_GRANULARITY_4KILO),
    // 32 Bit Data segment
    GDT_ENTRY_M( 0,
        0xFFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA | GDT_ACCESS_DATA_WRITABLE,
        GDT_FLAGS_32BIT | GDT_FLAGS_GRANULARITY_4KILO)
};

GDT_DESCRIPTOR GDT_Descriptor = { (sizeof(GDT_Entries) - 1), GDT_Entries};

void i686_GDT_Initalize(uint16_t arg__Code, uint16_t arg__Data) {
    uint16_t CodeSeg = arg__Code * 8;
    uint16_t DataSeg = arg__Data * 8;
    i686_Load_GDT(&GDT_Descriptor, CodeSeg, DataSeg);
}