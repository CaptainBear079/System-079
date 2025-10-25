#include "./../services/PMM.h"

// Start address of the system memory
extern void* SYSTEM_MEMORY__START;
// Address of the last two bytes of system memory
extern uint16_t* SYSTEM_MEMORY_SIGNATURE;
// Start address of the user memory
extern void* USER_APP_MEMORY__START;
// Address of the last two bytes of user memory
extern uint16_t* USER_APP_MEMORY_SIGNATURE;
// Cluster size in bytes
extern uint16_t CLUSTER_SIZE_IN_BYTE;

// Allocate a cluster of system memory
void* __SYS__C_ALLOC();