#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Token_ {
    int type;
    int value;
} Token;

enum TOKEN_TYPE {
    TOKEN_TYPE_BOOL,        // 1 Bit
    TOKEN_TYPE_CHAR,        // 1 Byte number or character
    TOKEN_TYPE_UCHAR,       // Unsigned 1 byte numbers only
    TOKEN_TYPE_SHORT,       // 2 byte number
    TOKEN_TYPE_USHORT,      // Unsigned 2 byte number
    TOKEN_TYPE_INT,         // 4 byte number
    TOKEN_TYPE_UINT,        // Unsigned 4 byte number
    TOKEN_TYPE_LONG,        // 6 byte number
    TOKEN_TYPE_ULONG,       // Unsigned 6 byte number
    TOKEN_TYPE_LONGLONG,    // 8 byte number
    TOKEN_TYPE_ULONGLONG,   // Unsigned 8 byte number
    TOKEN_TYPE_FLOAT,       // 4 byte floating point number
    TOKEN_TYPE_UFLOAT,      // Unsigned 4 byte floating point number
    TOKEN_TYPE_DOUBLE,      // 8 byte floating point number
    TOKEN_TYPE_UDOUBLE,     // Unsigned 8 byte floating point number
    TOKEN_TYPE_LONGDOUBLE,  // 16 byte floating point number
    TOKEN_TYPE_ULONGDOUBLE, // Unsigned 16 byte floating point number
    TOKEN_TYPE_IDENTIFIER   // Identifier
};

int main(int argc, char* argv[]) {
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
            else if(c == (int)' ' || c == (int)'\n' || c == (int)'\t') {
                Token token;
                // Check type
                if(i == 4 && strncmp(code_buffer, "int", 3) == 0) {
                    token.type = TOKEN_TYPE_INT;
                }
            }
            switch(c) {
                case EOF: {
                    done = true;
                    goto REACHED_EOF;
                } break;
                case (int)'A': {
                    code_buffer[i] = c;
                    goto NEXT_TOKEN;
                }
                case (int)'Z': {
                    code_buffer[i] = c;
                    goto NEXT_TOKEN;
                }
                case (int)'a': {
                    code_buffer[i] = c;
                    goto NEXT_TOKEN;
                }
                case (int)'z': {
                    code_buffer[i] = c;
                    goto NEXT_TOKEN;
                } break;
                case (int)'n': {
                    code_buffer[i] = c;
                    goto NEXT_TOKEN;
                }
            }
            REACHED_EOF:
            break;
            NEXT_TOKEN:
        }
    }
    return 0;
}