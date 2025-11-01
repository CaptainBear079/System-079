#pragma once
#include <stdint.h>

typedef struct _GDT_ENTRY_ {
    uint16_t Limit_LOW;        // Limit low 16 Bit
    uint16_t Base_LOW;         // Base low 16 Bit
    uint8_t Base_MIDDLE;       // Base middle 8 Bit
    uint8_t Access;            // Access 8 Bit
    uint8_t Flags__Limit_HIGH; // Flags: G D/B L AVL, Limit high 4 Bit
    uint8_t Base_HIGH;         // Base high 8 Bit
} __attribute__((packed)) GDT_ENTRY;

typedef struct _GDT_DESCRIPTOR_ {
    uint16_t Size;      // Size of GDT (Entries)
    GDT_ENTRY* Address; // Address of GDT
} __attribute__((packed)) GDT_DESCRIPTOR;

typedef enum _GDT_ACCESS_ {
    GDT_ACCESS_ACCESSED__CPU_SET     = 0x01, // Got accessed (ignoring in most cases)

    GDT_ACCESS_CODE_READABLE         = 0x02, // Read from segment
    GDT_ACCESS_DATA_WRITABLE         = 0x02, // Write into segment

    GDT_ACCESS_CODE_CONFIRMING       = 0x04, // Confirming
    GDT_ACCESS_DATA_DIRECTION_NORMAL = 0x00, // Normal direction
    GDT_ACCESS_DATA_DIRECTION_DOWN   = 0x04, // Downwards direction like stack

    GDT_ACCESS_CODE                  = 0x08, // Code segment !!! TSS !!!
    GDT_ACCESS_DATA                  = 0x00, // Data segment !!! TSS !!!
    GDT_ACCESS_DESCRIPTOR_TSS        = 0x00, // Task State Segment

    GDT_ACCESS_RING0                 = 0x00, // Ring 0
    GDT_ACCESS_RING1                 = 0x20, // Ring 1
    GDT_ACCESS_RING2                 = 0x40, // Ring 2
    GDT_ACCESS_RING3                 = 0x60, // Ring 3

    GDT_ACCESS_PRESENT               = 0x80  // Segment present
} GDT_ACCESS;

typedef enum _GDT_FLAGS_ {
    GDT_FLAGS_AVL               = 0x0F, // Free for the OS

    GDT_FLAGS_64BIT             = 0x20, // 64 Bit segment
    GDT_FLAGS_32BIT             = 0x40, // 32 Bit segment
    GDT_FLAGS_16BIT             = 0x00, // 16 Bit segment

    GDT_FLAGS_GRANULARITY_1BYTE = 0x00, // In bytes
    GDT_FLAGS_GRANULARITY_4KILO = 0x80  // In 4 kilobyte blocks
} GDT_FLAGS;

#define GDT_LIMIT_LOW(limit)             (limit & 0xFFFF)
#define GDT_BASE_LOW(base)               (base & 0xFFFF)
#define GDT_BASE_MIDDLE(base)            ((base >> 16) & 0xFF)
#define GDT_FLAGS_LIMIT_HI(limit, flags) (((limit >> 16) & 0xF) | (flags & 0xF0))
#define GDT_BASE_HIGH(base)              ((base >> 24) & 0xFF)

#define GDT_ENTRY_M(base, limit, access, flags) {                     \
    GDT_LIMIT_LOW(limit),                                           \
    GDT_BASE_LOW(base),                                             \
    GDT_BASE_MIDDLE(base),                                          \
    access,                                                         \
    GDT_FLAGS_LIMIT_HI(limit, flags),                               \
    GDT_BASE_HIGH(base)                                             \
}

#ifdef __CPU_I686__
extern void __attribute__((cdecl)) i686_Load_GDT(GDT_DESCRIPTOR* GDT_Descriptor, uint16_t CodeSegment, uint16_t DataSegment);
void i686_GDT_Initalize(uint16_t arg__Code, uint16_t arg__Data);
#endif