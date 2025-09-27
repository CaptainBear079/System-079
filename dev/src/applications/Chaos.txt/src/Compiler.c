#include "./../../../bootloader/stage2/restdlibs/stdio.h"

#include <stdint.h>
#include <stdbool.h>

#define ARGUMENT (int)'-'

uint8_t Args = 0;

bool ReadArg(const char* arg) {
    if(*arg == "-32-free") {
        Args = Args || 0x1;
    }
    else {
        printf("ERROR: Argument \"%s\" does not exist. CHAOS EXIT CODE: %X", *arg, 0x2); // Argument not recognized
        return false;
    }
    printf("ERROR: Program left execution path (CODE VIEW) Funktion: %s CHAOS EXIT CODE: %X", "Chaos.txt Compiler F: ReadArg", 0x1); // Left execution path (CODE VIEW)
    return false;
}

int main(int argc, char* argv[]) {
    // Compiler for Chaos.txt
    int file_count;
    file_count = 0;
    int ifiles[30];
    for(int i = 1; i < argc; i++) {
        const char* carg = argv[i];
        int arg = (int)carg[0];
        switch(arg) {
            case ARGUMENT: {
                ReadArg(argv[i]);
            } break;
            default: {
                // Expect it to be a file
                ifiles[file_count] = i;
                file_count++;
            } break;
        }
    }
    for(int i = file_count; i >= 0; i--) {
        // Read file and Compile
        //FILE* fptr = fopen(argv[ifiles[i]], 'r');
    }
    return 1; // Leave without finishing
}