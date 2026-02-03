//
// System 079 C Standard Library stdio.h v1.0.0
// Date: 04 September 2025 Written by Captain Bear
// Email: hacklordcaptainbear@gmail.com
// GitHub: https://github.com/CaptainBear079/System-079
//
// By using, copying, or/and modifying this code, or the assembled machine code, you agree to the following:
// - You give credit to the original author(s)
// - You document all your changes
// - You don't remove or/and modify this notice
// - You add your own notice for your modifications and credit the changes author(s)
// - You agree to the extern license if there is one
// - You agree that the original author(s) do not hold any responsibility for your modifications
// - You are aware that this code is provided as-is without any warranty
// - You do not make money with it, when you want to make money with it, you have to ask for permission (email)
//      Note: Will be granted in most cases (as example: game development, most software, most cases of commercial use in operating systems), but you have to ask!
// - You do not make fun of this code or it's author(s), this also counts for the author(s) of changes and/or the users and community,
//      this includes any form of harassment or discrimination
// - You do not use this code or the assembled machine code for any illegal purposes
//
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
int extprintf(const char* format, ...);
int core_printf(const char* format, va_list args);
int clear_screen();
void set_pos(unsigned long int x, unsigned long int y);
#endif
