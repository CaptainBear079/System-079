#include "string.h"

// Copy SRC to DEST.
char* strcpy(char* __dest, const char* __src) {
    char* src = __src;
    char* dest = __dest;
    while(*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    return __dest;
}

// Compare S1 and S2.
int strcmp(const char* __s1, const char* __s2) {
    char* s1 = __s1;
    char* s2 = __s2;
    while(*s1 == *s2) {
        if(*s1 == '\0' || *s2 == '\0') {
            return 0;
        }
        s1++;
        s2++;
    }
    return 1;
}

// Return the length of S.
size_t strlen(const char* __s) {
    // Count characters
    size_t len = 0;
    char* str = __s;
    while(*str != '\0') {
        len++;
        str++;
    }
    return len;
}