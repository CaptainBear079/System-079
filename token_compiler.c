#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Token_ {
    int type;
    int value;
} Token;

typedef struct _FUNCTION_ {
    char* name;
    unsigned long long id;
} FUNCTION;

typedef struct _IDENTIFIER_ {
    char* name;
    unsigned long long id;
} IDENTIFIER;

typedef struct _COMPILER_ {
    int flags[1];
    FUNCTION** functions;
    IDENTIFIER** identifiers;
} COMPILER;

enum TOKEN_TYPE {
    TOKEN_TYPE_BOOL,           // 1 Bit
    TOKEN_TYPE_CHAR,           // 1 Byte number or character
    TOKEN_TYPE_UCHAR,          // Unsigned 1 byte numbers only
    TOKEN_TYPE_SHORT,          // 2 byte number
    TOKEN_TYPE_USHORT,         // Unsigned 2 byte number
    TOKEN_TYPE_INT,            // 4 byte number
    TOKEN_TYPE_UINT,           // Unsigned 4 byte number
    TOKEN_TYPE_LONG,           // 6 byte number
    TOKEN_TYPE_ULONG,          // Unsigned 6 byte number
    TOKEN_TYPE_LONGLONG,       // 8 byte number
    TOKEN_TYPE_ULONGLONG,      // Unsigned 8 byte number
    TOKEN_TYPE_FLOAT,          // 4 byte floating point number
    TOKEN_TYPE_UFLOAT,         // Unsigned 4 byte floating point number
    TOKEN_TYPE_DOUBLE,         // 8 byte floating point number
    TOKEN_TYPE_UDOUBLE,        // Unsigned 8 byte floating point number
    TOKEN_TYPE_LONGDOUBLE,     // 16 byte floating point number
    TOKEN_TYPE_ULONGDOUBLE,    // Unsigned 16 byte floating point number
    TOKEN_TYPE_POINTER,        // Pointer 4/8 byte depending on cpu bit mode
    TOKEN_TYPE_STRUCT,         // Structure (Pointer)
    TOKEN_TYPE_UNION,          // Union (Pointer) size = max member size
    TOKEN_TYPE_ENUM,           // Enum (4 byte number) named integer constants
    TOKEN_TYPE_NOT,            // Not/invertation operator
    TOKEN_TYPE_EQUALS,         // Equals operator '=='
    TOKEN_TYPE_NOT_EQUALS,     // Not equals operator '!='
    TOKEN_TYPE_LESS_THAN,      // Less than operator '<'
    TOKEN_TYPE_GREATER_THAN,   // Greater than operator '>'
    TOKEN_TYPE_LESS_EQUAL,     // Less than or equal operator '<='
    TOKEN_TYPE_GREATER_EQUAL,  // Greater than or equal operator '>='
    TOKEN_TYPE_SET,            // Assignment operator '='
    TOKEN_TYPE_IDENTIFIER_REF, // Reference to a identifier
    TOKEN_TYPE_FUNCTION_REF,   // Reference to a function
    TOKEN_TYPE_NUMBER,         // Number
    TOKEN_TYPE_IDENTIFIER      // Identifier
};

int MAX_FUNCTIONS = 250;
int MAX_IDENTIFIERS = 450;

int main(int argc, char* argv[]) {
    MAX_FUNCTIONS = atoi(argv[2]);
    FUNCTION* functions = malloc(MAX_FUNCTIONS * sizeof(FUNCTION));
    MAX_IDENTIFIERS = atoi(argv[3]);
    IDENTIFIER* identifiers = malloc(MAX_IDENTIFIERS * sizeof(IDENTIFIER));
    COMPILER compiler = { {0}, &functions, &identifiers };
    bool done = false;
    FILE* fptr = fopen(argv[1], "r");
    char c = '\0';
    Token* tokens = malloc(1024 * sizeof(Token));
    char* code_buffer = malloc(257 * sizeof(char));
    code_buffer[256] = '\0';
    while(!done) {
        // Read token
        for(int i = 0;i < 256;i++) {
            c = fgetc(fptr);
            if(c == EOF) {
                done = true;
                break;
            }
            else if(c >= (int)'A' && c < (int)'Z') {
                code_buffer[i] = c;
                continue;
            }
            else if(c >= (int)'a' && c < (int)'z') {
                code_buffer[i] = c;
                continue;
            }
            else if(c == (int)' ' || c == (int)';' || c == (int)'\n' || c == (int)'\t') {
                Token token;
                // Check token
                if(i == 4 && strncmp(code_buffer, "int", 3) == 0) {
                    token.type = TOKEN_TYPE_INT;
                }
                else if(i == 2 && strncmp(code_buffer, "=", 1) == 0) {
                    token.type = TOKEN_TYPE_SET;
                }
                else {
                    // Jump to the right path
                    switch(compiler.flags[0]) {
                        // 0 is empty incase it never got set or wrong initalized
                        // Identifier for new variable expected
                        case 1: {
                            // Go trough every identifier and look for the string
                            int x = 0;
                            while(x < MAX_IDENTIFIERS) {
                                if(strncmp(code_buffer, identifiers[x].name, i) == 0) {
                                    printf("[ERROR] Identifier redefinition. \'%s\' is already defined.", code_buffer);
                                }
                                x++;
                            }
                            // New identifier
                            if(x >= MAX_IDENTIFIERS) {
                                printf("[FATAL ERROR] Too many identifiers defined. please increase the max identifier count. \"-idc <count>\"");
                                return -1;
                            }
                            token.type = TOKEN_TYPE_IDENTIFIER;
                            identifiers[x].id = x;
                            identifiers[x].name = malloc((i + 1) * sizeof(char));
                            strncpy(identifiers[x].name, code_buffer, i);
                            identifiers[x].name[i] = '\0';
                            compiler.flags[0] = 2; // Expect assignment or end of statement
                            break;
                        }
                    }
                    // Go trough every function name and look for the string
                    for(int x = 0;x < MAX_FUNCTIONS;x++) {
                        if(strncmp(code_buffer, functions[x].name, i) == 0) {
                            token.type = TOKEN_TYPE_FUNCTION_REF;
                            token.value = functions[x].id;
                        }
                    }
                    // Go trough every identifier and look for the string
                    for(int x = 0;x < MAX_IDENTIFIERS;x++) {
                        if(strncmp(code_buffer, identifiers[x].name, i) == 0) {
                            token.type = TOKEN_TYPE_IDENTIFIER_REF;
                            token.value = identifiers[x].id;
                        }
                    }
                    // New identifier
                    token.type = TOKEN_TYPE_IDENTIFIER;
                }
            }
            else if(i >= 255) {
                // Same like in the last if in the else
            }
        }
    }
    return 0;
}