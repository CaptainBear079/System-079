#include "stdlib.h"

// Get Bootloader Memory Map. DO NOT USE THIS FUNCTION AFTER OVERWRITING BIOS DATA!
int __SYS_Init_Memory_Map(const bool UseBIOS) {
	if(UseBIOS) {
		// Get BIOS Memory Map
		_MEM_MAP_ENTRY_ NewEntry;
		uint32_t ebx = 0;
		uint32_t temp_ebx = 0;
		uint64_t temp = 0;
		if(asm_m16_int0x15_E820h_Get_BIOS_Memory_Map(16, 0x0)) {
			return 0x0001; // Error getting BIOS Memory Map
		}

		// Get EBX register
		ebx = INT15hE820_EBX;
        temp_ebx = ebx;

		for(int i = temp_ebx; i <= INT15hE820_EBX - 1; i--) {
			// Start
			NewEntry.Start = (void*)(uint64_t)(MemoryMapBuffer[0 + i] & 0xFFFFFFFFFFFFFFFF);
			temp = MemoryMapBuffer[1 + i];
			NewEntry.Start = (void*)(((uint64_t)NewEntry.Start) + (uint64_t)((temp << 32) & 0xFFFFFFFFFFFFFFFF));

			// Size
			NewEntry.Size = (uint64_t)(MemoryMapBuffer[2 + i] & 0xFFFFFFFFFFFFFFFF);
			temp = MemoryMapBuffer[3 + i];
			NewEntry.Size = NewEntry.Size + ((temp << 32) & 0xFFFFFFFFFFFFFFFF);

            // End
            NewEntry.End = (void*)(uint64_t)NewEntry.Start + NewEntry.Size;

			// Type
			NewEntry.Type = MemoryMapBuffer[4 + i];

            // Save and print entry
            MemMap.entries[MemMap.EntryCount] = NewEntry;
            printf("Entry//: Size: %u, Start: %p, End: %p, Type: %u\n", NewEntry.Size, NewEntry.Start, NewEntry.End, NewEntry.Type);
		}

        // Update MemMap Info
	}
	return 0;
}

// Allocate SIZE bytes of memory.
void* malloc(size_t __size) {
    if(__size > MemMap.BiggestFreeMemory) {
        printf("[ERROR] No memory block big enough avalible. Input size: %u", __size);
        return NULL;
    }
    // Create a new entry
    _MEM_MAP_ENTRY_ NewEntry = { (uint8_t)1, (uint_t)__size, (uint_t)0, (uint_t)0 };
    void* TempStart = 0x0;
    void* TempEnd = 0x0;
    uint_t CurrentEntry = 0;
    void* CurrentAddress = 0x0;
    while(CurrentEntry < MemMap.EntryCount || CurrentAddress <= (MemMap.BiggestAddress - __size)) {
        if(CurrentEntry < MemMap.EntryCount && MemMap.entries[CurrentEntry].Start == CurrentAddress) {
            CurrentAddress = MemMap.entries[CurrentEntry].End + 1;
            CurrentEntry++;
        }
        else {
            TempStart = CurrentAddress;
            if(CurrentEntry < MemMap.EntryCount && MemMap.entries[CurrentEntry].Start < (TempStart + __size)) {
                TempEnd = TempStart + __size;
                goto MemoryFound;
            }
            else if(CurrentEntry >= MemMap.EntryCount && MemMap.BiggestAddress <= (TempStart + __size)) {
                TempEnd = TempStart + __size;
                goto MemoryFound;
            }
        }
    }
    if(CurrentAddress >= MemMap.BiggestAddress) {
        printf("[ERROR] No Free memory found. Input size: %u", __size);
        return NULL;
    }
    MemoryFound:
    NewEntry.Start = TempStart;
    NewEntry.End = TempEnd;
    MemMap.entries[MemMap.EntryCount] = NewEntry;
    return (void*)NewEntry.Start;
}