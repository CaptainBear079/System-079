#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _Token_ {
	char* str;
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

typedef struct _ARG_LIST_ {
	int type;
	char* name;
} ARG_LIST;

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
	// Flags
	int flags[3];                  // 0 = Interpretation path, 1 = Functions complexity level (0 = no functions, 1 = functions used), 2 = Current section (0 = source, 1 = script)
	bool bflags[2];                // 0 = In-/Outside function (true = In-, false = Outside), 1 = Optimize and translate
	bool bflagsArgs[1];            // 0 = Assemble Flag

	// Meta data
	int column, line;              // Position
	FILE* fptr;                    // File

	// Changing data
	int token_start;               // Start column of the currently read token
	int current_identifier;        // The next free identifier entry
	int current_token_index;       // The next free token entry

	// Assembler meta data
	int assembler_length;          // Default: 4;
	char* ASSEMBLER;               // Default: "nasm"
	int current_assembler_flag;
	int* assembler_flags_length;
	char** assembler_flags;
	int temp_assembly_file_length; // Default: 14
	char* temp_assembly_file;      // Default: "./temp_asm.asm"

	// Limits
	int MAX_TOKENS; // Max tokens: default 1024
	int MAX_FUNCTIONS; // Max functions: default 250
	int MAX_IDENTIFIERS; // Max identifiers: default 450
	int MAX_ERRORS; // Max errors before terminating compiler: default 500

	// Compilation data
	Token* tokens;
	FUNCTION* functions;
	IDENTIFIER* identifiers;
	int* list_of_types;
	char* code_buffer;
	CODE_OBJECT* pre_compiled_code; // Pre-compiled code (Next translation and optimization)
} COMPILER;

enum CODE_OBJECT_TYPE {
	CODE_OBJECT_TYPE_BOOL,              // 1 Bit
	CODE_OBJECT_TYPE_CHAR,              // 1 Byte number or character
	CODE_OBJECT_TYPE_UCHAR,             // Unsigned 1 byte numbers only
	CODE_OBJECT_TYPE_SHORT,             // 2 byte number
	CODE_OBJECT_TYPE_USHORT,            // Unsigned 2 byte number
	CODE_OBJECT_TYPE_INT,               // 4 byte number
	CODE_OBJECT_TYPE_UINT,              // Unsigned 4 byte number
	CODE_OBJECT_TYPE_LONG,              // 6 byte number
	CODE_OBJECT_TYPE_ULONG,             // Unsigned 6 byte number
	CODE_OBJECT_TYPE_LONGLONG,          // 8 byte number
	CODE_OBJECT_TYPE_ULONGLONG,         // Unsigned 8 byte number
	CODE_OBJECT_TYPE_FLOAT,             // 4 byte floating point number
	CODE_OBJECT_TYPE_UFLOAT,            // Unsigned 4 byte floating point number
	CODE_OBJECT_TYPE_DOUBLE,            // 8 byte floating point number
	CODE_OBJECT_TYPE_UDOUBLE,           // Unsigned 8 byte floating point number
	CODE_OBJECT_TYPE_LONGDOUBLE,        // 16 byte floating point number
	CODE_OBJECT_TYPE_ULONGDOUBLE,       // Unsigned 16 byte floating point number
	CODE_OBJECT_TYPE_POINTER,           // Pointer 4/8 byte depending on cpu bit mode
	CODE_OBJECT_TYPE_STRUCT,            // Structure (Pointer)
	CODE_OBJECT_TYPE_UNION,             // Union (Pointer) size = max member size
	CODE_OBJECT_TYPE_ENUM,              // Enum (4 byte number) named integer constants
	CODE_OBJECT_TYPE_NOT,               // Not/invertation operator
	CODE_OBJECT_TYPE_EQUALS,            // Equals operator '=='
	CODE_OBJECT_TYPE_NOT_EQUALS,        // Not equals operator '!='
	CODE_OBJECT_TYPE_LESS_THAN,         // Less than operator '<'
	CODE_OBJECT_TYPE_GREATER_THAN,      // Greater than operator '>'
	CODE_OBJECT_TYPE_LESS_EQUAL,        // Less than or equal operator '<='
	CODE_OBJECT_TYPE_GREATER_EQUAL,     // Greater than or equal operator '>='
	CODE_OBJECT_TYPE_SET,               // Assignment operator '='
	CODE_OBJECT_TYPE_IDENTIFIER_REF,    // Reference to a identifier
	CODE_OBJECT_TYPE_FUNCTION_REF,      // Reference to a function
	CODE_OBJECT_TYPE_NUMBER,            // Number
	CODE_OBJECT_TYPE_IDENTIFIER,        // Identifier
	CODE_OBJECT_TYPE_CALCULATION,       // Calculation
	CODE_OBJECT_TYPE_ARG_LIST,          // Argument list 
	CODE_OBJECT_TYPE_CODE_BLOCK,        // Code block
};

#define C compiler
#define fileName argv[1]

int PreProcessor(COMPILER* compiler) {
	// Pre-processor directives:
	// - include:
	//   Includes code from other files.
	//   Written: #include <file.h/file.soft/file.source/file.script>
	// - define:
	//   Defines the meaning for a token.
	//   Written: #define identifier value
	// - if:
	//   If but for preprocessor macros.
	//   Written: #if condition
	// - elif:
	//   Continuation of #if.
	//   Written: #elif condition
	// - else:
	//   Written: #else
	// - endif:
	//   Ends #if and #elif.
	//   Written: #endif
	// - INFO:
	//   Metadata for the executable.
	//   - TITLE:
	//     Sets the title of the window. (MOSTLY WINDOWS GUI)
	//   - USER:
	//     Sets which user/user type is needed to run the program.
	//     Options: "Root/Admin", "Any", "ProgramUser"
	//   - MAIN:
	//     Sets the entry point.
	//   - RANDOM:
	//     Sets if extended randomization algorithms are used.
	//     Options: true, false
	//   - DEFINES_REQ:
	//     Sets which defines are needed to compile the program.

	// Create/Reopen a temporary file for the pre-processed code
	FILE* preProcessedFile = fopen("./temp_preprocessed.pre", "w+");
	if(preProcessedFile == NULL) {
		printf("[ERROR] Could not create temporary pre-processed file.\n");
		return -1;
	}

	// Find a pre-processor directive
	int c = fgetc(C->fptr);
	while(c != EOF) {
		if(c == (int)'#') {
			// Read directive
			char directive_buffer[32] = { 0 };
			c = fgetc(C->fptr);
			switch(c) {
				case (int)'i': {
					//
				}
			}
		}
		else {
			// Write character to pre-processed file
			fputc(c, preProcessedFile);
		}
		c = fgetc(C->fptr);
	}
	fclose(C->fptr);
	C->fptr = preProcessedFile;
	rewind(C->fptr);
	return 0;
}

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
					break;
				}
				if(C->tokens[i].str[0] == '=') {}
				else if(C->tokens[i].str[0] == '(') {
					// Read argument list
				}
			}
		}
	}
}

int Translate(COMPILER* compiler) {
	switch(C->pre_compiled_code->type) {
		case CODE_OBJECT_TYPE_INT: {
			// IDK.
		} break;
		default: {
			printf("[ERROR] Kinks.\n");
		} break;
	}
}

int Assemble(COMPILER* compiler) {
	// Build assembler call
	// Calculate length
	int length = C->assembler_length;
	for(int i = 0;i < C->current_assembler_flag;i++) {
		length = length + C->assembler_flags_length[i];
	}
	length = length + C->temp_assembly_file_length;

	// Allocate call buffer
	char* assembler_call = malloc(length * sizeof(char));
	char* temp = assembler_call;
	if(temp == NULL || assembler_call == NULL) {
		return 1;
	}

	// Write call
	strncpy(temp, C->ASSEMBLER, C->assembler_length);
	temp = temp + C->assembler_length;
	for(int i = 0;i < C->current_assembler_flag;i++) {
		strncpy(temp, C->assembler_flags[i], C->assembler_flags_length[i]);
		temp = temp + C->assembler_flags_length[i];
	}
	strncpy(temp, C->temp_assembly_file, C->temp_assembly_file_length);

	// Call assembler
	system(assembler_call);
	return 0;
}

int main(int argc, char* argv[]) {
	// DEBUG: Argument chack
	if(argc < 6) {
		printf("[ERROR] Not enough arguments.\n<file> <max functions> <max identifiers> <max errors before terminating> <max tokens> <assemble>\n");
		return -1;
	}

	// Initalize compiler object
	COMPILER compiler = {
		/* Flags */ { 0, 0, 0 }, { false, false }, { false },
		/* Meta data */ 1, 1, fopen(fileName, "r"),
		/* Changing data */ 1, 0, 0,
		/* Assembler meta data*/ 4, "nasm", 1, NULL, NULL, 14, "./temp_asm.asm",
		/* Limits */ 1024, 250, 450, 500,
		/* Compilation data */ NULL, NULL, NULL, NULL, NULL, NULL
	};
	C.MAX_TOKENS = atoi(argv[5]);
	C.MAX_FUNCTIONS = atoi(argv[2]);
	C.MAX_IDENTIFIERS = atoi(argv[3]);
	C.MAX_ERRORS = atoi(argv[4]);
	C.tokens = malloc(C.MAX_TOKENS * sizeof(Token));
	C.functions = malloc(C.MAX_FUNCTIONS * sizeof(FUNCTION));
	C.identifiers = malloc(C.MAX_IDENTIFIERS * sizeof(IDENTIFIER));
	C.code_buffer = malloc(256 * sizeof(char));
	C.code_buffer[256] = '\0';
	bool done = false;                // While flag
	int c = '\0';                     // Character holder

	// Compiling chain
	while(!done) {
		// Pre-processor
		PreProcessor(&C);

		// Process into token
		// 1.Get the first character from fptr
		c = fgetc(C.fptr);
		// 2.Read trough file
		for(int i = 0;i <= 255;i++) {
			// Is End Of File reached
			if(c == EOF) {
				// Set while flag
				done = true;
				// Save last token
				if(C.code_buffer[0] != '\0') {
					C.tokens[C.current_token_index].str = malloc(i * sizeof(char));
					strncpy(C.tokens[C.current_token_index].str, C.code_buffer, (i - 1));
					C.tokens[C.current_token_index].str[i] = '\0';
					C.tokens[C.current_token_index].column = C.token_start;
					C.tokens[C.current_token_index].line = C.line;
					C.token_start = C.column;
					C.current_token_index++;
					for(int j = 0;j <= i;j++) {
						C.code_buffer[j] = '\0';
					}
					i = -1;
				}
				// Leave for-loop
				break;
			}
			else if(i >= 255) {
				// Max token size reached
				// Save token
				if(C.code_buffer[0] != '\0') {
					C.tokens[C.current_token_index].str = malloc(i * sizeof(char));
					strncpy(C.tokens[C.current_token_index].str, C.code_buffer, (i - 1));
					C.tokens[C.current_token_index].str[i] = '\0';
					C.tokens[C.current_token_index].column = C.token_start;
					C.tokens[C.current_token_index].line = C.line;
					C.token_start = C.column;
					C.current_token_index++;
					for(int j = 0;j <= i;j++) {
						C.code_buffer[j] = '\0';
					}
					i = -1;
				}
				else {
					// FATAL ERROR
					printf("FATAL ERROR. PROGRAM TERMINATED.");
					return -1;
				}
			}
			else if(c >= (int)'A' && c <= (int)'Z' || c >= (int)'a' && c <= (int)'z' || c == (int)'_' || c >= (int)'0' && c <= (int)'9' || c == (int)'=') {
				// Save character
				C.code_buffer[i] = c;
				C.column++;
				
			}
			else if(c == (int)' ' || c == (int)';' || c == (int)'\n' || c == (int)'\t') {
				// Token ending character
				// Save token
				if(C.code_buffer[0] != '\0') {
					C.tokens[C.current_token_index].str = malloc(i * sizeof(char));
					strncpy(C.tokens[C.current_token_index].str, C.code_buffer, (i - 1));
					C.tokens[C.current_token_index].str[i] = '\0';
					C.tokens[C.current_token_index].column = C.token_start;
					C.tokens[C.current_token_index].line = C.line;
					C.token_start = C.column;
					C.current_token_index++;
					for(int j = 0;j <= i;j++) {
						C.code_buffer[j] = '\0';
					}
					i = -1;
				}
			}
			else {
				// Other characters (Argument list, calculations, access, ...)
				switch(c) {
					// Calculation Opening/Caller Arg List Opening
					case (int)'(': {
						C.tokens[C.current_token_index].str = malloc((i - 1) * sizeof(char));
						strncpy(C.tokens[C.current_token_index].str, C.code_buffer, (i - 1));
						C.tokens[C.current_token_index].str[i - 1] = '\0';
						C.tokens[C.current_token_index].column = C.token_start;
						C.tokens[C.current_token_index].line = C.line;
						C.token_start = C.column;
						C.current_token_index++;
						for(int j = 0;j < i;j++) {
							C.code_buffer[j] = '\0';
						}
						C.code_buffer[0] = c;
						C.column++;
						i = 0;
					} break;
					// Calculation Closing/ Caller Arg List Closing
					case (int)')': {
						C.code_buffer[i] = c;
						C.column++;
						i++;
						C.tokens[C.current_token_index].str = malloc(i * sizeof(char));
						strncpy(C.tokens[C.current_token_index].str, C.code_buffer, i);
						C.tokens[C.current_token_index].str[i] = '\0';
						C.tokens[C.current_token_index].column = C.token_start;
						C.tokens[C.current_token_index].line = C.line;
						C.token_start = C.column;
						C.current_token_index++;
						for(int j = 0;j < i;j++) {
							C.code_buffer[j] = '\0';
						}
						i = -1;
					} break;
					// Code Object/Object
					case (int)'{':
					case (int)'}': {
						C.tokens[C.current_token_index].str = malloc((i - 1) * sizeof(char));
						strncpy(C.tokens[C.current_token_index].str, C.code_buffer, (i - 1));
						C.tokens[C.current_token_index].str[i - 1] = '\0';
						C.tokens[C.current_token_index].column = C.token_start;
						C.tokens[C.current_token_index].line = C.line;
						C.token_start = C.column;
						C.current_token_index++;
						for(int j = 0;j < i;j++) {
							C.code_buffer[j] = '\0';
						}
						i = 0;
						C.code_buffer[i] = c;
						C.column++;
						i++;
						C.tokens[C.current_token_index].str = malloc(i * sizeof(char));
						strncpy(C.tokens[C.current_token_index].str, C.code_buffer, i);
						C.tokens[C.current_token_index].str[i] = '\0';
						C.tokens[C.current_token_index].column = C.token_start;
						C.tokens[C.current_token_index].line = C.line;
						C.token_start = C.column;
						C.current_token_index++;
						for(int j = 0;j < i;j++) {
							C.code_buffer[j] = '\0';
						}
						i = -1;
					} break;
					default: {} break;
				}
				printf("[FATAL ERROR] Unsupported character, at %d:%d.", C.line, C.column);
			}
			c = fgetc(C.fptr);
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