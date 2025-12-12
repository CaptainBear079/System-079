#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Token_ {
	char* str;
	int type;
	int value;
	int line;
	int column;
} Token;

typedef struct _FUNCTION_ {
	char* name;
	unsigned long long id;
} FUNCTION;

typedef struct _IDENTIFIER_ {
	char* name;
	unsigned long long id;
} IDENTIFIER;

typedef struct _CODE_OBJECT_ {
	// Represents a line of code
	// Supported:
	//  - variable declaration
	//  - function declaration
	int type; // See CODE_OBJECT_TYPE enum
	union {
		char* identifier; // Identifier of variables or functions
	} CODE_OBJECT_DATA;
} CODE_OBJECT;

typedef struct _COMPILER_ {
	int flags[3]; // 0 = Interpretation path, 1 = Functions complexity level (0 = no functions, 1 = functions used), 2 = Current section (0 = source, 1 = script)
	bool bflags[2]; // 0 = In-/Outside function (true = In-, false = Outside), 1 = Optimize and translate
	bool bflagsArgs[1]; // 0 = Assemble Flag
	int column, line;
	int token_start; // Column
	int current_identifier;
	int current_token_index;
	int current_error;
	int MAX_TOKENS; // Max tokens: default 1024
	int MAX_FUNCTIONS; // Max functions: default 250
	int MAX_IDENTIFIERS; // Max identifiers: default 450
	int MAX_ERRORS; // Max errors before terminating compiler: default 500
	Token* tokens;
	FUNCTION* functions;
	IDENTIFIER* identifiers;
	char* code_buffer;
	CODE_OBJECT* pre_compiled_code; // Pre-compiled code (Next translation and optimization)
} COMPILER;

enum TOKEN_TYPE {
	TOKEN_TYPE_BOOL,              // 1 Bit
	TOKEN_TYPE_CHAR,              // 1 Byte number or character
	TOKEN_TYPE_UCHAR,             // Unsigned 1 byte numbers only
	TOKEN_TYPE_SHORT,             // 2 byte number
	TOKEN_TYPE_USHORT,            // Unsigned 2 byte number
	TOKEN_TYPE_INT,               // 4 byte number
	TOKEN_TYPE_UINT,              // Unsigned 4 byte number
	TOKEN_TYPE_LONG,              // 6 byte number
	TOKEN_TYPE_ULONG,             // Unsigned 6 byte number
	TOKEN_TYPE_LONGLONG,          // 8 byte number
	TOKEN_TYPE_ULONGLONG,         // Unsigned 8 byte number
	TOKEN_TYPE_FLOAT,             // 4 byte floating point number
	TOKEN_TYPE_UFLOAT,            // Unsigned 4 byte floating point number
	TOKEN_TYPE_DOUBLE,            // 8 byte floating point number
	TOKEN_TYPE_UDOUBLE,           // Unsigned 8 byte floating point number
	TOKEN_TYPE_LONGDOUBLE,        // 16 byte floating point number
	TOKEN_TYPE_ULONGDOUBLE,       // Unsigned 16 byte floating point number
	TOKEN_TYPE_POINTER,           // Pointer 4/8 byte depending on cpu bit mode
	TOKEN_TYPE_STRUCT,            // Structure (Pointer)
	TOKEN_TYPE_UNION,             // Union (Pointer) size = max member size
	TOKEN_TYPE_ENUM,              // Enum (4 byte number) named integer constants
	TOKEN_TYPE_NOT,               // Not/invertation operator
	TOKEN_TYPE_EQUALS,            // Equals operator '=='
	TOKEN_TYPE_NOT_EQUALS,        // Not equals operator '!='
	TOKEN_TYPE_LESS_THAN,         // Less than operator '<'
	TOKEN_TYPE_GREATER_THAN,      // Greater than operator '>'
	TOKEN_TYPE_LESS_EQUAL,        // Less than or equal operator '<='
	TOKEN_TYPE_GREATER_EQUAL,     // Greater than or equal operator '>='
	TOKEN_TYPE_SET,               // Assignment operator '='
	TOKEN_TYPE_IDENTIFIER_REF,    // Reference to a identifier
	TOKEN_TYPE_FUNCTION_REF,      // Reference to a function
	TOKEN_TYPE_NUMBER,            // Number
	TOKEN_TYPE_IDENTIFIER,        // Identifier
	TOKEN_TYPE_CALCULATION_START, // Calculation start
	TOKEN_TYPE_CALCULATION_END,   // Calculation end
	TOKEN_TYPE_ARG_LIST_START,    // Argument list start
	TOKEN_TYPE_ARG_LIST_END,      // Argument list end
	TOKEN_TYPE_CODE_BLOCK_START,  // Code block start
	TOKEN_TYPE_CODE_BLOCK_END     // Code block end
};

#define C compiler

/*int checkToken(char** code_buffer, int* i, COMPILER* compiler, int* c) {
	printf("New token: %s, line: %d, column: %d\n", *code_buffer, compiler->line, (compiler->column - (compiler->column - compiler->token_start)));
	// Check token
	if(strcmp(*code_buffer, "int") == 0) {
		compiler->tokens[compiler->current_token_index].type = TOKEN_TYPE_INT;
		compiler->current_token_index++;
		compiler->flags[0] = 1; // Expect new variable identifier
	}
	else if(strcmp(*code_buffer, "=") == 0) {
		if(compiler->flags[0] == 2) {
			compiler->tokens[compiler->current_token_index].type = TOKEN_TYPE_SET;
			compiler->current_token_index++;
			compiler->flags[0] = 3; // Expect value
		}
	}
	else if(strcmp(*code_buffer, "!") == 0) {
		compiler->tokens[compiler->current_token_index].type = TOKEN_TYPE_NOT;
		compiler->current_token_index++;
		compiler->flags[0] = 4;
	}
	else {
		int x = 0;
		// Go trough every function name and look for the string
		if(compiler->flags[1] != 0) {
			while(x < C->MAX_FUNCTIONS) {
				if(strncmp(*code_buffer, compiler->functions[x].name, *i) == 0) {
					if(compiler->flags[0] == 1) {
						C->ERRORS[compiler->current_error] = 1; // Can't name a variable after a function Code: R1
					}
				}
				x++;
			}
			x = 0;
		}
		if(!(compiler->current_identifier <= 0)) {
			// Go trough every identifier and look for the string
			while(x < C->MAX_IDENTIFIERS) {
				if(strncmp(*code_buffer, compiler->identifiers[x].name, *i) == 0) {
					C->ERRORS[compiler->current_error] = 2; // Identifier already defined Code: R2
				}
				x++;
			}
		}
		// New identifier
		if(x >= C->MAX_IDENTIFIERS) {
			printf("[FATAL ERROR] Too many identifiers defined. Please increase the max identifier count. \"-idc <count>\"");
			return -1;
		}
		compiler->tokens[compiler->current_token_index].type = TOKEN_TYPE_IDENTIFIER;
		compiler->identifiers[compiler->current_identifier].id = compiler->current_identifier;
		compiler->identifiers[compiler->current_identifier].name = malloc((*i + 1) * sizeof(char));
		strncpy(compiler->identifiers[compiler->current_identifier].name, *code_buffer, *i);
		compiler->identifiers[compiler->current_identifier].name[*i] = '\0';
		compiler->current_token_index++;
		compiler->flags[0] = 2; // Expect assignment or end of statement
	}
	compiler->tokens[compiler->current_token_index].column = compiler->token_start;
	compiler->tokens[compiler->current_token_index].line = compiler->line;
	if(*c == (int)' ') {
		compiler->column++;
	}
	else if(*c == (int)'\n') {
		compiler->column = 0;
		compiler->line++;
	}
	else if(*c == (int)'\t') {
		compiler->column += 4;
	}
	compiler->token_start = compiler->column;
	return 0;
}*/

int ParseCode(COMPILER* compiler) {
	// Variables
	CODE_OBJECT temp_code_object;

	// Split into sections ("__SEC_SCRIPT", "__SEC_SOURCE")
	C->flags[2] = 0; // Current section: 0 = source, 1 = script
	for(int i = 0;i < C->current_token_index;i++) {
		// Check if change to script section is made
		if(strcmp(C->tokens[i].str, "__SEC_SCRIPT") == 0) {
			C->flags[2] = 1; // Set section to script
			continue;
		}
		else if(strcmp(C->tokens[i].str, "__SEC_SOURCE") == 0) {
			C->flags[2] = 0; // Section didn't change
			continue;
		}
		// Inside or outside function
		else if(C->bflags[0]) {
			// Currently parsing inside a function
			if(strcmp(C->tokens[i].str, "int") == 0) {
				// Integer variable
			}
		}
		else {
			// Currently parsing outside a function
			if(strcmp(C->tokens[i].str, "int") == 0) {
				// Global integer variable or function declaration
				i++;
				if(!(i < C->current_token_index)) {
					// Error
					C->bflags[1] = false;
					C->current_error++;
					// Print error
					printf("[ERROR] Definition incomplete. End of file.\n");
					return -1;
				}
				
				if(isalpha(C->tokens[i].str[0]) || C->tokens[i].str[0] == '_') {
					// Save the name of the variable/function in temp_code_object
					temp_code_object.CODE_OBJECT_DATA.identifier = C->tokens[i].str;
				}

				i++;
				if(!(i < C->current_token_index)) {
					// Error
					C->bflags[1] = false;
					C->current_error++;
					// Print errore
				}
			}
		}
	}
}

int Assemble(COMPILER* compiler) {
	// Call assembler
}

int main(int argc, char* argv[]) {
	if(argc < 5) {
		printf("[ERROR] Not enough arguments.\n<file> <max functions> <max identifiers> <max errors before terminating> <max tokens> <assemble>\n");
		return -1;
	}
	
	COMPILER compiler = { { 0, 0 }, { false, false }, 1, 1, 1, 0, 0, 0, 1024, 250, 450, 500, NULL, NULL, NULL, NULL, NULL };
	C.MAX_TOKENS = atoi(argv[5]);
	C.MAX_FUNCTIONS = atoi(argv[2]);
	C.MAX_IDENTIFIERS = atoi(argv[3]);
	C.MAX_ERRORS = atoi(argv[4]);
	C.tokens = malloc(C.MAX_TOKENS * sizeof(Token));
	C.functions = malloc(C.MAX_FUNCTIONS * sizeof(FUNCTION));
	C.identifiers = malloc(C.MAX_IDENTIFIERS * sizeof(IDENTIFIER));
	C.code_buffer = malloc(256 * sizeof(char));
	C.code_buffer[256] = '\0';
	bool done = false;
	FILE* fptr = fopen(argv[1], "r");
	int c = '\0';
	while(!done) {
		// Process into token
		c = fgetc(fptr);
		for(int i = 0;i <= 255;i++) {
			if(c == EOF) {
				done = true;
				if(C.code_buffer[0] != '\0') {
					C.tokens[C.current_token_index].str = malloc(i * sizeof(char));
					strncpy(C.tokens[C.current_token_index].str, C.code_buffer, (i - 1));
					C.tokens[C.current_token_index].str[i] = '\0';
					for(int j = 0;j <= i;j++) {
						C.code_buffer[j] = '\0';
					}
					i = -1;
				}
				break;
			}
			else if(i >= 255) {
				if(C.code_buffer[0] != '\0') {
					C.tokens[C.current_token_index].str = malloc(i * sizeof(char));
					strncpy(C.tokens[C.current_token_index].str, C.code_buffer, (i - 1));
					C.tokens[C.current_token_index].str[i] = '\0';
					for(int j = 0;j <= i;j++) {
						C.code_buffer[j] = '\0';
					}
					i = -1;
				}
			}
			else if(c >= (int)'A' && c <= (int)'Z' || c >= (int)'a' && c <= (int)'z' || c == (int)'_' || c >= (int)'0' && c <= (int)'9' || c == (int)'=') {
				C.code_buffer[i] = c;
				C.column++;
				
			}
			else if(c == (int)' ' || c == (int)';' || c == (int)'\n' || c == (int)'\t') {
				if(C.code_buffer[0] != '\0') {
					C.tokens[C.current_token_index].str = malloc(i * sizeof(char));
					strncpy(C.tokens[C.current_token_index].str, C.code_buffer, (i - 1));
					C.tokens[C.current_token_index].str[i] = '\0';
					for(int j = 0;j <= i;j++) {
						C.code_buffer[j] = '\0';
					}
					i = -1;
				}
			}
			else if(c == (int)'(') {
				C.code_buffer[i] = c;
				C.column++;
				C.tokens[C.current_token_index].str = malloc(i * sizeof(char));
				strncpy(C.tokens[C.current_token_index].str, C.code_buffer, i);
				C.tokens[C.current_token_index].str[i] = '\0';
				for(int j = 0;j <= i;j++) {
						C.code_buffer[j] = '\0';
					}
					i = -1;
			}
			else {
				printf("[FATAL ERROR] Unsupported character, at %d:%d.", C.line, C.column);
			}
			c = fgetc(fptr);
		}

		// Parse code
		ParseCode(&C);

		// Translate
		Translate(&C);

		
		if(C.bflagsArgs[0]) {
			// Assemble
			Assemble(&C);
		}
	}
	return 0;
}