#define __32BIT__ // 32 Bit Protected Mode for the libraries

#include <stdint.h>
#include "./language_support/C/Standard_Libraries/stdio.h"
#include "./other.h"

extern uint8_t __bss_start;
extern uint8_t __end;

uint8_t* ScreenBuffer = (uint8_t*)0xB8000;
uint32_t __SYS_SCREEN_X = 0;
uint32_t __SYS_SCREEN_Y = 0;

void __attribute__((section(".entry"))) kernel0() {
    // Clear BSS section (uninitialized global/static variables)
    memset(&__bss_start, 0, (uint16_t)((&__end) - (&__bss_start)));

    // Kernel: Init code
    clear_screen();
    printf("Welcome to System 079 Kernel 0.0.1!\n");

    for(;;);

    printf("[FATAL ERROR] Kernel loop exited! Please restart the PC before damage occurs.\n");
    return; // Should never reach this point
}