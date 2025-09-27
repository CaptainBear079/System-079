#include "stdio.h"

#ifdef __32BIT__
#include "./../asm.h"
// Text output functions
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define DEFAULT_COLOR 0x07

uint8_t* ScreenBuffer = (uint8_t*)0xB8000;
extern uint32_t __SYS_SCREEN_X;
extern uint32_t __SYS_SCREEN_Y;
const char HexCharsUpper[] = "0123456789ABCDEF";
const char HexCharsLower[] = "0123456789abcdef";

void print_char(char c, uint8_t color);
void print_color(unsigned long int x, unsigned long int y, uint8_t color);
void print_c(unsigned long int x, unsigned long int y, char c);
int print_string(const char* str);
int print_decimal(int value);
int print_unsigned_decimal(unsigned int value);
int print_octal(unsigned int value);
int print_hexadecimal(uint32_t value, bool uppercase);
int print_float(float value);
int print_float_hex(float value, bool uppercase);
int print_scientific(float value, bool uppercase);
int print_pointer(unsigned long int address);
int* specifier_switch(const char* format, va_list* args, specifier_modifiers* modifiers, int* char_count);
int modifier_get_width(const char* format);
int modifier_get_precision(const char* format);

int print_signed(unsigned long int value, int radix, bool uppercase);
int print_unsigned(unsigned long int value, int radix, bool uppercase);

int SetCursorPosition(unsigned long int x, unsigned long int y);

int SetCursorPosition(unsigned long int x, unsigned long int y) {
    int pos = y * SCREEN_WIDTH + x;

    asm_output(0x3D4, 0x0F);
    asm_output(0x3D5, (uint8_t)(pos & 0xFF));
    asm_output(0x3D4, 0x0E);
    asm_output(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    return 0;
}

int clear_screen() {
    __SYS_SCREEN_X = 0;
    __SYS_SCREEN_Y = 0;
    for(unsigned long int y = 0; y < SCREEN_HEIGHT; y++) {
        for(unsigned long int x = 0; x < SCREEN_WIDTH; x++) {
            // Clear Screen Buffer and set color on Background black and Foreground white
            print_c(x, y, '\0');
            print_color(x, y, DEFAULT_COLOR);
        }
    }
    SetCursorPosition(0, 0);
    return 0;
}

int extprintf(uint32_t DisTextAPrf_ScreenX, uint32_t DisTextAPrf_ScreenY, const char* format, ...) {
    va_list args;
    va_start(args, format);
    // Set ScreenX and ScreenY
    __SYS_SCREEN_X = DisTextAPrf_ScreenX;
    __SYS_SCREEN_Y = DisTextAPrf_ScreenY;
    // Call core_printf with the arguments like normal call
    int ret = core_printf(format, args);
    va_end(args);
    return ret;
}

int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int ret = core_printf(format, args);
    va_end(args);
    return ret;
}
int core_printf(const char* format, va_list args) {
    // Implementation of printf using ScreenBuffer, ScreenX, and ScreenY
    int char_count = 0;
    const char* ptr = format;
    specifier_modifiers modifiers = {0};
    while(*ptr) {
        switch(*ptr) {
            case '%': {
                // Specifier
                int* i = specifier_switch(ptr, &args, &modifiers, &char_count);
                char_count += *i;
            } break;

            case '\n': {
                // New Line
                __SYS_SCREEN_X = 0;
                __SYS_SCREEN_Y++;
                SetCursorPosition(__SYS_SCREEN_X, __SYS_SCREEN_Y);
            } break;
            case '\r': {
                // Carriage Return
                __SYS_SCREEN_X = 0;
                SetCursorPosition(__SYS_SCREEN_X, __SYS_SCREEN_Y);
            } break;
            case '\t': {
                // Tab
                for(int i = 4; i != 0; i--) {
                    print_char('\0', DEFAULT_COLOR);
                }
            } break;
            case '\b': {
                // Backspace
                __SYS_SCREEN_X--;
                print_char('\0', DEFAULT_COLOR);
                __SYS_SCREEN_X--;
                SetCursorPosition(__SYS_SCREEN_X, __SYS_SCREEN_Y);
            } break;
            case '\\': {
                // Backslash
                print_char('\\', DEFAULT_COLOR);
            } break;
            case '\'': {
                // Single Quote
                print_char('\'', DEFAULT_COLOR);
            } break;
            case '\"': {
                // Double Quote
                print_char('\"', DEFAULT_COLOR);
            } break;
            case '\0': {
                // Null Character
                print_char('\0', DEFAULT_COLOR);
            } break;
            case '\v': {
                // Vertical Tab
                __SYS_SCREEN_X = 0;
                for(int i = 4; i >= 1; i--) {
                    for(int j = 0; j < SCREEN_WIDTH; j++) {
                        print_char('\0', DEFAULT_COLOR);
                    }
                }
            } break;
            /*case '\o': {
                // Octal
            } break;
            case '\x': {
                // Hexadecimal
            } break;*/

            default: {
                // Other
                print_char(*ptr, DEFAULT_COLOR);
            } break;
        }
        ptr++;
    }
    return 0;
}

int* specifier_switch(const char* format, va_list* args, specifier_modifiers* modifiers, int* char_count) {
    // Implementation of specifier_switch
    format++;
    switch(*format) {
        case 'd': {
            // Decimal Value
            int value = (int)va_arg(*args, int);
            *char_count += print_decimal(value);
        } break;
        case 'i': {
            // Decimal Value (same as %d)
            int value = (int)va_arg(*args, int);
            *char_count += print_decimal(value);
        } break;
        case 'u': {
            // Unsigned Decimal Value
            unsigned int value = (unsigned int)va_arg(*args, int);
            *char_count += print_unsigned_decimal(value);
        } break;
        case 'o': {
            // Octal Value
            unsigned int value = (unsigned int)va_arg(*args, int);
            *char_count += print_octal(value);
        } break;
        case 'x': {
            // Hexadecimal Value (lowercase)
            uint32_t value = (uint32_t)va_arg(*args, uint32_t);
            *char_count += print_hexadecimal(value, 0);
        } break;
        case 'X': {
            // Hexadecimal Value (uppercase)
            uint32_t value = (uint32_t)va_arg(*args, uint32_t);
            *char_count += print_hexadecimal(value, 1);
        } break;
        case 'g': {
            // Shorter of %f and %e
            float value = (float)va_arg(*args, int);
            *char_count += print_float(value);
        } break;
        case 'G': {
            // Shorter of %f and %E
            float value = (float)va_arg(*args, int);
            *char_count += print_float(value);
        } break;
        case 'a': {
            // Hex value floating point lowercase
            // Note: This is C99 specific, but should be usable for any System 079 based systems and programs
            float value = (float)va_arg(*args, int);
            *char_count += print_float_hex(value, 0);
        } break;
        case 'A': {
            // Hex value floating point uppercase
            // Note: This is C99 specific, but should be usable for any System 079 based systems and programs
            float value = (float)va_arg(*args, int);
            *char_count += print_float_hex(value, 1);
        } break;
        case 'e': {
            // Scientific notation (lowercase)
            float value = (float)va_arg(*args, int);
            *char_count += print_scientific(value, 0);
        } break;
        case 'E': {
            // Scientific notation (uppercase)
            float value = (float)va_arg(*args, int);
            *char_count += print_scientific(value, 1);
        } break;
        case 'f': {
            // Floating Point Value
            float value = (float)va_arg(*args, int);
            *char_count += print_float(value);
        } break;
        case 'c': {
            *char_count += 1;
            // Character
            char value = (char)va_arg(*args, int);
            print_char(value, DEFAULT_COLOR);
        } break;
        case 's': {
            // String
            const char* value = (const char*)va_arg(*args, const char*);
            *char_count += print_string(value);
        } break;
        case 'p': {
            // Pointer Address
            unsigned long long value = (unsigned long long)va_arg(*args, unsigned long long);
            *char_count += print_pointer(value);
        }break;
        case 'n': {
            // Number of characters printed so far
            int value = *char_count;
            *char_count += print_decimal(value);
        } break;
        case '%': {
            *char_count += 1;
            // Literal Percent Sign
            print_char('%', DEFAULT_COLOR);
        } break;
        case '-': {
            format++;
            modifiers->left_justify = true;
            modifiers->width = modifier_get_width(format);
            specifier_switch(format, args, modifiers, char_count);
        } break;
        case '+': {
            format++;
            modifiers->show_sign = true;
            specifier_switch(format, args, modifiers, char_count);
        } break;
        case '0': {
            format++;
            modifiers->zero_pad = true;
            modifiers->width = modifier_get_width(format);
            specifier_switch(format, args, modifiers, char_count);
        } break;
        case '.': {
            format++;
            modifiers->float_precision = true;
            modifiers->precision = modifier_get_precision(format);
            specifier_switch(format, args, modifiers, char_count);
        }
        case '#': {
            format++;
            modifiers->hex_prefix = true;
            specifier_switch(format, args, modifiers, char_count);
        } break;
        case 'h': {
            format++;
            modifiers->short_integer = true;
            specifier_switch(format, args, modifiers, char_count);
        } break;
        case 'l': {
            if (*(format + 1) == 'l') {
                modifiers->long_long_integer = true;
                format += 2;
            } else {
                modifiers->long_integer = true;
                format++;
            }
            specifier_switch(format, args, modifiers, char_count);
        } break;
        default: {
            modifiers->width = modifier_get_width(format);
            specifier_switch(format, args, modifiers, char_count);
        } break;
    }
    return char_count;
}

int modifier_get_width(const char* format) {
    int width = 0;
    while (*format >= '0' && *format <= '9') {
        width = width * 10 + (*format - '0');
        format++;
    }
    return width;
}

int modifier_get_precision(const char* format) {
    int precision = 0;
    while (*format >= '0' && *format <= '9') {
        precision = precision * 10 + (*format - '0');
        format++;
    }
    return precision;
}

void print_char(char c, uint8_t color) {
    print_color(__SYS_SCREEN_X, __SYS_SCREEN_Y, color);
    print_c(__SYS_SCREEN_X, __SYS_SCREEN_Y, c);
    __SYS_SCREEN_X++;
    if(__SYS_SCREEN_X > SCREEN_WIDTH - 1) {
        __SYS_SCREEN_X = 0;
        __SYS_SCREEN_Y++;
    }
    if(__SYS_SCREEN_Y > SCREEN_HEIGHT - 1) {
        __SYS_SCREEN_Y = 0;
    }
    SetCursorPosition(__SYS_SCREEN_X, __SYS_SCREEN_Y);
}
void print_c(unsigned long int x, unsigned long int y, char c) {
    // Implementation of print_c
    ScreenBuffer[2 * (y * SCREEN_WIDTH + x)] = c;
}

void print_color(unsigned long int x, unsigned long int y, uint8_t color) {
    // Implementation of print_color
    ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}

char get_c(unsigned long int x, unsigned long int y) {
    // Implementation of print_c
    return ScreenBuffer[2 * (y * SCREEN_WIDTH + x)];
}

uint8_t get_color(unsigned long int x, unsigned long int y) {
    // Implementation of print_color
    return ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1];
}

int print_string(const char* str) {
    int char_count = 0;
    while (*str) {
        print_char(*str, DEFAULT_COLOR);
        str++;
        char_count++;
    }
    return char_count;
}

int print_decimal(int value) {
    // Implementation of print_decimal
    int length = 0;
    // Convert integer to string
    length += print_signed(value, 10, false);
    return length;
}

int print_unsigned_decimal(unsigned int value) {
    // Implementation of print_unsigned_decimal
    int length = 0;
    // Convert integer to string
    length += print_unsigned(value, 10, false);
    return length;
}

int print_hexadecimal(uint32_t value, bool uppercase) {
    // Implementation of print_hexadecimal
    int length = 0;
    // Convert integer to string
    char c;
    uint32_t temp;
    const char* HexChars = NULL;

    if(uppercase) {
        HexChars = HexCharsUpper;
    }
    else {
        HexChars = HexCharsLower;
    }

    for(uint32_t i = 28; i >= 0;i++) {
        temp = ((value >> i || 0xFFFFFFF0) - 0xFFFFFFF0);
        if(temp != 0) {
            if(temp > 15) {
                return -1;
            }
            c = HexChars[temp];
            print_char(c, DEFAULT_COLOR);
            i++;
            i++;
            i++;
        }
        else {
            i++;
            i++;
            i++;
        }
    }

    return length;
}


int print_octal(unsigned int value) {
    // Implementation of print_octal
    int length = 0;
    // Convert integer to string
    length += print_unsigned(value, 8, false);
    print_char('o', DEFAULT_COLOR);
    return length + 1;
}

int print_pointer(unsigned long int address) {
    int length = 2;
    print_string("0x");
    length += print_unsigned(address, 16, true);
    return length;
}

int print_float(float value) {
    // Implementation of print_float
    int length = 0;
    // Convert float to string
    length += print_signed((int)value, 10, false);
    print_char('.', DEFAULT_COLOR);
    length += print_unsigned((unsigned int)(value * 100) % 100, 10, false);
    return length + 3;
}

int print_float_hex(float value, bool uppercase) {
    // Implementation of print_float_hex
    int length = 0;
    // Convert float to hexadecimal string
    length += print_signed((int)value, 16, uppercase);
    print_char('.', DEFAULT_COLOR);
    length += print_unsigned((unsigned int)(value * 100) % 100, 16, uppercase);
    return length + 3;
}

int print_scientific(float value, bool uppercase) {
    // Implementation of print_scientific
    int length = 0;
    // Convert float to scientific notation string
    length += print_signed((int)value, 10, false);
    print_char('.', DEFAULT_COLOR);
    length += print_unsigned((unsigned int)(value * 100) % 100, 10, false);
    return length + 3;
}

int print_signed(unsigned long int value, int radix, bool uppercase) {
    if(value < 0) {
        print_char('-', DEFAULT_COLOR);
        int length = print_unsigned(-value, radix, uppercase);
        return length + 1;
    }
    else {
        int length = print_unsigned(value, radix, uppercase);
        return length;
    }
    return -1;
}

int print_unsigned(unsigned long int value, int radix, bool uppercase) {
    char buffer[32];
    int pos = 0;
    int length = 0;
    const char* HexChars = NULL;

    if(uppercase) {
        HexChars = HexCharsUpper;
    }
    else {
        HexChars = HexCharsLower;
    }

    do {
        unsigned long int rem = value % 10;
        value /= radix;
        buffer[pos++] = HexChars[rem];
    } while (value > 0);

    while(--pos >= 0) {
        print_char(buffer[pos], DEFAULT_COLOR);
        length++;
    }
    return length;
}
#endif
