#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct _specifier_modifiers_ {
    bool left_justify;      // '-'
    bool show_sign;         // '+'
    bool zero_pad;          // '0'
    bool float_precision;   // '.'
    bool hex_prefix;        // '#'
    bool short_integer;     // 'h'
    bool long_integer;      // 'l'
    bool long_long_integer; // 'll'
    int width;           // Minimum field width
    int precision;       // Precision for floating point
} specifier_modifiers;

// 32 Bit Protected Mode I/O
#ifdef __32BIT__
// Text output functions
int printf(const char* format, ...);
int extprintf(uint32_t DisTextAPrf_ScreenX, uint32_t DisTextAPrf_ScreenY, const char* format, ...);
int core_printf(const char* format, va_list args);
int clear_screen();
#endif
