#include "stdlib.h"

void* malloc(size_t __size) {
    if(__size > MemMap.BigestFreeMemory) {
        printf("[ERROR] No memory block big enough avalible. Input size: %u", __size);
        return NULL;
    }
    // Create a new entry
    _MEM_MAP_ENTRY_ NewEntry = { (uint8_t)1, (uint_t)__size, (uint_t)0, (uint_t)0 };
    uint_t TempStart = 0;
    uint_t TempEnd = 0;
    uint_t CurrentEntry = 0;
    uint_t CurrentAddress = 0;
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
}