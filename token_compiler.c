#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _Token_ {
	char* str;
	int length;
	int line;
	int column;
} Token;

typedef struct _ARG_LIST_ {
	int type;
	char* name;
} ARG_LIST;

typedef struct _IDENTIFIER_ {
	char* name;
	unsigned long long id;
} IDENTIFIER;

typedef struct _FUNCTION_ {
	char* name;
	unsigned long long id;
	ARG_LIST* args;
	IDENTIFIER* local_identifiers;
} FUNCTION;

typedef struct _DEFINE_ {
	char* name;
	char* value;
} DEFINE;

typedef struct _PCC__BOOL_ {
	bool global;                       // Indicates if the boolean is global or local (0 = local, 1 = global)
	char* identifier;                  // Name of the boolean
	char* asm_identifier;              // Name that will be used in assembly
	bool value;                        // Standard value of the boolean
	int group[2];                      // First index (0) saves in which byte group (every eight bool becomes a byte group) the bool is saved, second (1) saves which bit
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__BOOL;

typedef struct _PCC__INT_ {
	bool global;                       // Indicates if the integer is global or local (0 = local, 1 = global)
	int type;                          // Indicates the size of the integer (char, short, int, long int, long long int/long long)
	char* identifier;                  // Name of the integer
	char* asm_identifier;              // Name that will be used in assembly
	long long value;                   // Standard value of the integer
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__INT;

typedef struct _PCC__UINT_ {
	bool global;                       // Indicates if the integer is global or local (0 = local, 1 = global)
	int type;                          // Indicates the size of the integer (uchar, ushort, uint, ulong int, ulong long int/ulong long)
	char* identifier;                  // Name of the integer
	char* asm_identifier;              // Name that will be used in assembly
	unsigned long long value;          // Standard value of the integer
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__UINT;

typedef struct _PCC__FLOAT_ {
	bool global;                       // Indicates if the float is global or local (0 = local, 1 = global)
	int type;                          // Indicates the size of the float (float, double, long double)
	char* identifier;                  // Name of the float
	char* asm_identifier;              // Name that will be used in assembly
	long double value;                 // Standard value of the float
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__FLOAT;

typedef struct _PCC__UFLOAT_ {
	bool global;                       // Indicates if the float is global or local (0 = local, 1 = global)
	int type;                          // Indicates the size of the float (ufloat, udouble, ulong double)
	char* identifier;                  // Name of the float
	char* asm_identifier;              // Name that will be used in assembly
	long double value;                 // Standard value of the float
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__UFLOAT;

typedef struct _PCC__STRINGS_ {
	bool global;                       // Indicates if the char is global or local (0 = local, 1 = global)
	int type;                          // Indicates if string or single character
	char* identifier;                  // Name of the char
	char* asm_identifier;              // Name that will be used in assembly
	union _STRINGS_VALUE_ {                    // Standard value of the char
		char value;
		unsigned char uvalue;
		char* str_value;
	} STRINGS_VALUE;
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__STRINGS;

typedef struct _PCC__POINTER_ {
	bool global;                       // Indicates if the pointer is global or local (0 = local, 1 = global)
	bool array;                        // Indicates if the pointer is an array pointer.
	int type;                          // Indicates the type the pointer points to.
	char* identifier;                  // Name of the pointer
	char* asm_identifier;              // Name that will be used in assembly
	union _POINTER_VALUE_ { // Standard value of the pointer
		unsigned long long value;
		unsigned long long* array_values;
	} POINTER_VALUE;
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__POINTER;

typedef struct _PCC__CODE_BLOCK_ {
	int return_type;                   // Type of the return value
	int start_index;                   // Start index in pre-compiled code
	int end_index;                     // End index in pre-compiled code
	int function_index;                // Index of the function in the function table
	char* identifier;                  // Name of the function/code block
	char* asm_identifier;              // Name that will be used in assembly
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC_CODE_BLOCK;

typedef struct _CODE_OBJECT_ {
	// Represents a line of code
	// Supported:
	//  - variable declaration
	//  - function declaration
	int type; // See CODE_OBJECT_TYPE enum
	union CODE_OBJECT_DATA_ {
		PCC__BOOL* _bool;
		PCC__INT* _int;
		PCC__UINT* _uint;
		PCC__FLOAT* _float;
		PCC__UFLOAT* _ufloat;
		PCC__STRINGS* _string;
		PCC__POINTER* _pointer;
		PCC_CODE_BLOCK* _code_block;
	} CODE_OBJECT_DATA;
} CODE_OBJECT;

typedef struct _COMPILER_ {
	// Flags
	int flags[3];                   // 0 = Interpretation path, 1 = Functions complexity level (0 = no functions, 1 = functions used), 2 = Current section (0 = source, 1 = script)
	bool bflags[4];                 // 0 = In-/Outside function (true = In-, false = Outside), 1 = Optimize and translate, 2 = First error flag, 3 = End not set
	bool bflagsArgs[3];             // 0 = Assemble Flag, 1 = List tokens (DEBBUG), 2 = Long return method (false = jump to end, true = delete stack frame and use 'ret')

	// Meta data
	int column, line;               // Position
	char* fName;                    // Name of compiled file
	FILE* fptr;                     // File
	int pcc_entries;

	// Changing data
	int token_start;                // Start column of the currently read token
	int current_function;           // The next free function entry
	int current_identifier;         // The next free identifier entry
	int current_define;             // The next free define entry
	int current_token_index;        // The next free token entry

	// Assembler meta data
	int assembler_length;           // Default: 4;
	char* ASSEMBLER;                // Default: "nasm"
	int current_assembler_flag;
	int* assembler_flags_length;
	char** assembler_flags;
	int temp_assembly_file_length;  // Default: 14
	char* temp_assembly_file;       // Default: "./build/ChaosLangCompiler/temp_asm.asm"

	// Limits
	int MAX_TOKENS;                 // Max tokens: default 1024
	int MAX_FUNCTIONS;              // Max functions: default 250
	int MAX_IDENTIFIERS;            // Max identifiers: default 450
	int MAX_ERRORS;                 // Max errors before terminating compiler: default 500
	int MAX_DEFINES;                // Max defines: default 250
	int MAX_ASM_ID;                 // Max assembly identifiers: default 700

	// Compilation data
	FILE* temp_script_file;         // Temporary script file for script section
	FILE* temp_assembly;            // Temporary assembly file for assembler output
	Token* tokens;
	FUNCTION* functions;
	IDENTIFIER* identifiers;
	DEFINE* defines;
	int* list_of_types;
	char* code_buffer;
	CODE_OBJECT* pre_compiled_code; // Pre-compiled code (Next translation and optimization)
	char** asm_identifier_list;     // All identifiers used in assembly
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
	CODE_OBJECT_TYPE_POINTER_INT,       // Pointer to integer
	CODE_OBJECT_TYPE_ARRAY,             // Array (Pointer) size = element size * number of elements
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
	CODE_OBJECT_TYPE_RETURN,            // Return statement
	CODE_OBJECT_TYPE_IDENTIFIER_REF,    // Reference to a identifier
	CODE_OBJECT_TYPE_FUNCTION_REF,      // Reference to a function
	CODE_OBJECT_TYPE_NUMBER,            // Number
	CODE_OBJECT_TYPE_IDENTIFIER,        // Identifier
	CODE_OBJECT_TYPE_FUNCTION,          // Function declaration (needs ARG_LIST and CODE_BLOCK)
	CODE_OBJECT_TYPE_CALCULATION,       // Calculation
	CODE_OBJECT_TYPE_ARG_LIST,          // Argument list 
	CODE_OBJECT_TYPE_CODE_BLOCK,        // Code block
};

#define C compiler

int convert_str_to_int(char* str) {
	int result = 0;
	bool negative = false;
	int start_index = 0;

	if(str[0] == '-') {
		negative = true;
		start_index = 1;
	}

	for(int i = start_index; str[i] != '\0'; i++) {
		if(str[i] >= '0' && str[i] <= '9') {
			result = result * 10 + (str[i] - '0');
		}
		else {
			break;
		}
	}

	if(negative) {
		result = -result;
	}

	return result;
}

int convert_str_to_ullong(char* str) {
	unsigned long long result = 0;
	bool negative = false;
	int start_index = 0;

	if(str[0] == '-') {
		negative = true;
		start_index = 1;
	}

	for(int i = start_index; str[i] != '\0'; i++) {
		if(str[i] >= '0' && str[i] <= '9') {
			result = result * 10 + (str[i] - '0');
		}
		else {
			break;
		}
	}

	if(negative) {
		result = (unsigned long long)(-((long long)result));
	}

	return result;
}

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
	//     Options: "Root/Root" (SYS079 bootloader program only), "Root/Admin", "User/Any", "Background/ProgramUser"
	//   - MAIN:
	//     Sets the entry point.
	//   - RANDOM:
	//     Sets if extended randomization algorithms are used.
	//     Options: true, false
	//   - DEFINES_REQ:
	//     Sets which defines are needed to compile the program.

	// Create/Reopen a temporary file for the pre-processed code
	FILE* preProcessedFile = fopen("./build/ChaosLangCompiler/temp_preprocessed.pre", "w+");
	if(preProcessedFile == NULL) {
		printf("[ERROR] Could not create temporary pre-processed file.\n");
		return -1;
	}
	FILE* originalFile = C->fptr;

	// Find a pre-processor directive
	bool included_file = false;
	int c = fgetc(C->fptr);
	do {
		included_file = false;
		while(c != EOF) {
			if(c == (int)'#') {
				// Read directive
				char directive_buffer[33] = { 0 };
				directive_buffer[32] = '\0';
				c = fgetc(C->fptr);
				for(int i = 0; isalpha(c) && i < 32; i++) {
					if(c == EOF) {
						printf("[ERROR] Unexpected end of file in pre-processor directive.\n");
						return -1;
					}
					directive_buffer[i] = (char)c;
					c = fgetc(C->fptr);
				}

				if(strcmp(directive_buffer, "include") == 0) {
					// Include file
					included_file = true;
					// Skip whitespace
					while(c == (int)' ' || c == (int)'\t') {
						c = fgetc(C->fptr);
					}
				
					// Read file name
					char include_file_name[256] = { 0 };
					include_file_name[255] = '\0';
					int index = 0;
					if(c == (int)'"' || c == (int)'<') {
						int end_char = (c == (int)'"') ? (int)'"' : (int)'>';
						c = fgetc(C->fptr);
						while(c != end_char && c != EOF && index < 255) {
							include_file_name[index++] = (char)c;
							c = fgetc(C->fptr);
						}
						include_file_name[index] = '\0';
						if(c != end_char) {
							printf("[ERROR] Unterminated include file name.\n");
							return -1;
						}
					}
					else {
						printf("[ERROR] Invalid include directive format.\n");
						return -1;
					}

					// Open and read the include file
					FILE* include_file = fopen(include_file_name, "r");
					if(include_file == NULL) {
						printf("[ERROR] Could not open include file: %s\n", include_file_name);
						return -1;
					}
					int inc_c = fgetc(include_file);
					while(inc_c != EOF) {
						fputc(inc_c, preProcessedFile);
						inc_c = fgetc(include_file);
					}
					fclose(include_file);
				}
				else if(strcmp(directive_buffer, "define") == 0) {
					// Define macro
					// Skip whitespace
					while(c == (int)' ' || c == (int)'\t') {
						c = fgetc(C->fptr);
					}

					// Read identifier
					char define_identifier[64] = { 0 };
					define_identifier[63] = '\0';
					int index = 0;
					while((isalnum(c) || c == (int)'_') && c != EOF && index < 63) {
						define_identifier[index++] = (char)c;
						c = fgetc(C->fptr);
					}
					define_identifier[index] = '\0';
				
					// Skip whitespace
					while(c == (int)' ' || c == (int)'\t') {
						c = fgetc(C->fptr);
					}

					// Read value
					char define_value[256] = { 0 };
					define_value[255] = '\0';
					index = 0;
					while(c != EOF && index < 255) {
						define_value[index++] = (char)c;
						c = fgetc(C->fptr);
						if(c == (int)'\n') {
							if(define_value[index - 1] == '\\') {
								c = fgetc(C->fptr);
							}
							else {
								break;
							}
						}
					}
					define_value[index] = '\0';

					// Store the define
					if(C->current_define < C->MAX_DEFINES) {
						C->defines[C->current_define].name = strdup(define_identifier);
						C->defines[C->current_define].value = strdup(define_value);
						C->current_define++;
					}
					else {
						printf("[ERROR] Maximum number of defines reached.\n");
						return -1;
					}
				}
				else {
					printf("[ERROR] Unknown pre-processor directive: %s\n", directive_buffer);
					return -1;
				}
			}
			else {
				// Write character to pre-processed file
				fputc(c, preProcessedFile);
			}
			c = fgetc(C->fptr);
		}
		C->fptr = preProcessedFile;
	} while(included_file);
	fclose(originalFile);
	C->fptr = preProcessedFile;
	rewind(C->fptr);
	return 0;
}

int ParseCode(COMPILER* compiler) {
	// Split into sections ("__SEC_SCRIPT", "__SEC_SOURCE")
	C->flags[2] = 0; // Current section: 0 = source, 1 = script
	for(int i = 0;i < C->current_token_index;i++) {
		if(!(i < C->current_token_index)) {
			return 0;
		}
		// Check if change to script section is made
		else if(strcmp(C->tokens[i].str, "__SEC_SCRIPT") == 0) {
			C->flags[2] = 1; // Set section to script
			continue;
		}
		else if(strcmp(C->tokens[i].str, "__SEC_SOURCE") == 0) {
			C->flags[2] = 0; // Section didn't change
			continue;
		}
		else if(C->flags[2]) {
			// Create/Reopen temporary script file
			C->temp_script_file = fopen("./temp_script.script", "a+");
			if(C->temp_script_file == NULL) {
				printf("[ERROR] Could not create temporary script file.\n");
				return -1;
			}
			// Write token to script file
			fprintf(C->temp_script_file, "%s ", C->tokens[i].str);

			fclose(C->temp_script_file);
		}
		// Inside or outside function
		else if(C->bflags[0]) {
			// Currently parsing inside a function
			if(strcmp(C->tokens[i].str, "int") == 0) {
				// Local integer variable declaration
				i++;
				if(!(i < C->current_token_index)) {
					// Error
					C->bflags[1] = false;
					// Print error
					printf("[ERROR] Definition incomplete. End of file.\n");
					return -1;
				}

				PCC__INT _int = {0};
				
				if(isalpha(C->tokens[i].str[0]) || C->tokens[i].str[0] == '_') {
					// Save the name of the variable/function in temp_code_object
					C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._int->identifier = C->tokens[i].str;
				}

				i++;
				if(!(i < C->current_token_index)) {
					// Error
					C->bflags[1] = false;
					// Print error
					printf("[ERROR] Definition incomplete. End of file.\n");
					return -1;
				}
				if(strcmp(C->tokens[i].str, "=") == 0) {
					i++;
					if(!(i < C->current_token_index)) {
						// Error
						C->bflags[1] = false;
						// Print error
						printf("[ERROR] Definition incomplete. End of file.\n");
						return -1;
					}
					C->pre_compiled_code[C->pcc_entries].type = CODE_OBJECT_TYPE_INT;
					// "Set" or "Push to Address"
					if(strcmp(C->tokens[i].str, ">") == 0) {
						// Push to Address
						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}
						C->pre_compiled_code[C->pcc_entries].type = CODE_OBJECT_TYPE_POINTER_INT;
						C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._int->target_address = convert_str_to_ullong(C->tokens[i].str);

						// Check for command end
						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}

						if(strcmp(C->tokens[i].str, ";") == 0) {
							C->pcc_entries++;
							continue;
						}
					}
					else {
						// Set
						// Get length of number string
						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}
						int length = 0;
						for(int j = 0;C->tokens[i].str[j] != '\0';j++) {
							length++;
						}

						// Convert to integer number
						C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._int->value = convert_str_to_int(C->tokens[i].str);

						// Check for command end
						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}

						if(strcmp(C->tokens[i].str, ";") == 0) {
							C->pcc_entries++;
							continue;
						}
					}
				}
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

				char* temp_identifier = C->tokens[i].str;
				
				if(isalpha(C->tokens[i].str[0]) || C->tokens[i].str[0] == '_') {
					// Save the name of the variable/function in temp_code_object
					C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._int->identifier = temp_identifier;
				}

				i++;
				if(!(i < C->current_token_index)) {
					// Error
					C->bflags[1] = false;
					// Print error
					printf("[ERROR] Definition incomplete. End of file.\n");
					return -1;
				}
				if(strcmp(C->tokens[i].str, "=") == 0) {
					i++;
					if(!(i < C->current_token_index)) {
						// Error
						C->bflags[1] = false;
						// Print error
						printf("[ERROR] Definition incomplete. End of file.\n");
						return -1;
					}
					C->pre_compiled_code[C->pcc_entries].type = CODE_OBJECT_TYPE_INT;
					// "Set" or "Push to Address"
					if(strcmp(C->tokens[i].str, ">") == 0) {
						// Push to Address
						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}
						C->pre_compiled_code[C->pcc_entries].type = CODE_OBJECT_TYPE_POINTER_INT;
						C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._int->target_address = convert_str_to_ullong(C->tokens[i].str);

						// Check for command end
						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}

						if(strcmp(C->tokens[i].str, ";") == 0) {
							C->pcc_entries++;
							continue;
						}
					}
					else {
						// Set
						// Get length of number string
						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}
						int length = 0;
						for(int j = 0;C->tokens[i].str[j] != '\0';j++) {
							length++;
						}

						// Convert to integer number
						C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._int->value = convert_str_to_int(C->tokens[i].str);

						// Check for command end
						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}

						if(strcmp(C->tokens[i].str, ";") == 0) {
							C->pcc_entries++;
							continue;
						}
					}
				}
				else if(strcmp(C->tokens[i].str, "(") == 0) {
					// Read argument list
					C->pre_compiled_code[C->pcc_entries].type = CODE_OBJECT_TYPE_FUNCTION;
					C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._code_block->return_type = CODE_OBJECT_TYPE_INT;
					C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._code_block->identifier = temp_identifier;
					C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._code_block->function_index = C->current_function;
					C->functions[C->current_function].name = temp_identifier;
					C->functions[C->current_function].id = C->pcc_entries;
					
					// Read args
					i++;
					if(!(i < C->current_token_index)) {
						// Error
						C->bflags[1] = false;
						// Print error
						printf("[ERROR] Definition incomplete. End of file.\n");
						return -1;
					}
					// Count args
					int arg_count = 1;
					for(int j = i;j < C->current_token_index;j++) {
						if(strcmp(C->tokens[j].str, ")") == 0) {
							break;
						}
						else if(strcmp(C->tokens[j].str, ",") == 0) {
							arg_count++;
						}
					}
					C->functions[C->current_function].args = malloc(arg_count * sizeof(ARG_LIST));
					if(C->functions[C->current_function].args == NULL) {
						return -1;
					}
					// Read args
					for(int j = 0;j < arg_count;j++) {
						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}
						// Save arg type
						if(strcmp(C->tokens[i].str, "int") == 0) {
							C->functions[C->current_function].args[j].type = CODE_OBJECT_TYPE_INT;
						}
						else {
							// Unsupported type
							C->bflags[1] = false;
							printf("[ERROR] Unsupported argument type: %s\n", C->tokens[i].str);
							return -1;
						}

						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}

						// Save arg name
						C->functions[C->current_function].args[j].name = strdup(C->tokens[i].str);

						i++;
						if(!(i < C->current_token_index)) {
							// Error
							C->bflags[1] = false;
							// Print error
							printf("[ERROR] Definition incomplete. End of file.\n");
							return -1;
						}
					}
					// Update function end
					C->pcc_entries++;
					C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._code_block->start_index = C->pcc_entries;
					while (strcmp(C->tokens[i].str, "{") != 0) {
						int curly_brace_count = 0;
						int j = i;
						while(strcmp(C->tokens[j].str, "}") != 0 && curly_brace_count <= 0) {
							j++;
							if(!(j < C->current_token_index)) {
								// Error
								C->bflags[1] = false;
								// Print error
								printf("[ERROR] Definition incomplete. End of file.\n");
								return -1;
							}
							if(strcmp(C->tokens[j].str, "{") == 0) {
								curly_brace_count++;
							}
							else if(strcmp(C->tokens[j].str, "}") == 0) {
								curly_brace_count--;
							}
						}
						C->bflags[0] = true; // Inside function
						C->bflags[3] = true; // End not set
						i++;
					}
					C->current_function++;
					
				}
			}
			else if(strcmp(C->tokens[i].str, "return") == 0) {
				// Return statement
				i++;
				if(!(i < C->current_token_index)) {
					// Error
					C->bflags[1] = false;
					// Print error
					printf("[ERROR] Definition incomplete. End of file.\n");
					return -1;
				}
				C->pre_compiled_code[C->pcc_entries].type = CODE_OBJECT_TYPE_RETURN;
				C->pre_compiled_code[C->pcc_entries].CODE_OBJECT_DATA._int->value = convert_str_to_int(C->tokens[i].str);
			}
		}
	}
	return 0;
}

int Translate(COMPILER* compiler) {
	// Open temporary assembly file
	C->temp_assembly = fopen(C->temp_assembly_file, "w+");
	if(!C->temp_assembly) {
		printf("[ERROR] Could not create/reopen temporary assembly file.\n");
		return -1;
	}
	// Write assembly header (no calling main for now, just zero out bss)
	fputs("section .data\nsection .text\nglobal _start\n_start:\n\tmov rdi, bss_start\n\tmov rcx, bss_end\n\tsub rcx, rdi\n\txor rax, rax\n\trep stosb", C->temp_assembly);
	// Reopen for appending
	fclose(C->temp_assembly);
	C->temp_assembly = fopen(C->temp_assembly_file, "a+");
	if(!C->temp_assembly) {
		printf("[ERROR] Could not create/reopen temporary assembly file.\n");
		return -1;
	}
	// Read global variable definitions
	for(int i = 0;i < C->pcc_entries;i++) {
		switch(C->pre_compiled_code[i].type) {
			case CODE_OBJECT_TYPE_INT: {
				// Add global variable of type integer
				// Check for standard value
				fprintf(C->temp_assembly, "\tmov [%s], 00100110b\n", C->pre_compiled_code[i].CODE_OBJECT_DATA._int->asm_identifier);
				if(C->pre_compiled_code[i].CODE_OBJECT_DATA._int->value != 0) {
					fprintf(C->temp_assembly, "\tmov [%s], %d\n", C->pre_compiled_code[i].CODE_OBJECT_DATA._int->asm_identifier, C->pre_compiled_code[i].CODE_OBJECT_DATA._int->value);
				}
			} break;
		}
	}
	for(int i = 0;i < C->pcc_entries;i++) {
		switch(C->pre_compiled_code[i].type) {
			case CODE_OBJECT_TYPE_INT: {
				// IDK.
			} break;
			case CODE_OBJECT_TYPE_RETURN: {
				//
			} break;
			case CODE_OBJECT_TYPE_FUNCTION: {
				//
			} break;
			default: {
				printf("[ERROR] Kinks.\n");
			} break;
		}
	}
	fclose(C->temp_assembly);
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
	
	// Clean up
	free(assembler_call);
	return 0;
}

int compile(char* fileName, int maxTokens, int maxFunctions,
	int maxIdentifiers, int maxErrors);

int main(int argc, char* argv[]) {
	// DEBUG: Argument chack
	if(argc < 6) {
		printf("[ERROR] Not enough arguments.\n<file> <max functions> <max identifiers> <max errors before terminating> <max tokens> <assemble>\n");
		return -1;
	}

	return compile(argv[1], atoi(argv[5]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
}

int compile(char* fileName, int maxTokens, int maxFunctions,
	int maxIdentifiers, int maxErrors) {
	// Initalize compiler object
	COMPILER compiler = {
		/* Flags */ { 0, 0, 0 }, { false, false }, { false },
		/* Meta data */ 1, 1, NULL, fopen(fileName, "r"), 0,
		/* Changing data */ 1, 0, 0, 0, 0,
		/* Assembler meta data*/ 4, NULL, 1, NULL, NULL, 14, NULL,
		/* Limits */ 1024, 250, 450, 500,250, 700,
		/* Compilation data */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	};
	C.fName = strdup(fileName);
	C.temp_assembly_file = strdup("./build/ChaosLangCompiler/temp_asm.asm");
	C.ASSEMBLER = strdup("nasm");
	C.MAX_TOKENS = maxTokens;
	C.MAX_FUNCTIONS = maxFunctions;
	C.MAX_IDENTIFIERS = maxIdentifiers;
	C.MAX_ERRORS = maxErrors;
	C.tokens = malloc(C.MAX_TOKENS * sizeof(Token));
	C.functions = malloc(C.MAX_FUNCTIONS * sizeof(FUNCTION));
	C.identifiers = malloc(C.MAX_IDENTIFIERS * sizeof(IDENTIFIER));
	C.code_buffer = malloc(257 * sizeof(char));
	C.code_buffer[256] = '\0';
	C.pre_compiled_code = malloc(2500 * sizeof(CODE_OBJECT));
	C.asm_identifier_list = malloc(C.MAX_ASM_ID * sizeof(char*));
	C.defines = malloc(C.MAX_DEFINES * sizeof(DEFINE));
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
					C.tokens[C.current_token_index].str = strdup(C.code_buffer);
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
					C.tokens[C.current_token_index].str = strdup(C.code_buffer);
					C.tokens[C.current_token_index].length = i + 1;
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
			else if(c >= (int)'A' && c <= (int)'Z' || c >= (int)'a' && c <= (int)'z' ||
					c == (int)'_' || c >= (int)'0' && c <= (int)'9'
					) {
				// Save character
				C.code_buffer[i] = c;
				C.column++;
				
			}
			else if(c == (int)' ' || c == (int)'\n' || c == (int)'\t') {
				// Token ending character
				// Save token
				if(C.code_buffer[0] != '\0') {
					C.tokens[C.current_token_index].str = strdup(C.code_buffer);
					C.tokens[C.current_token_index].length = i + 1;
					C.tokens[C.current_token_index].column = C.token_start;
					C.tokens[C.current_token_index].line = C.line;
					C.token_start = C.column;
					C.current_token_index++;
					for(int j = 0;j <= i;j++) {
						C.code_buffer[j] = '\0';
					}
					i = -1;
				}

				// Update position
				if(c == (int)' ') {
					C.column++;
				}
				else if(c == (int)'\n') {
					C.line++;
					C.column = 1;
				}
				else if(c == (int)'\t') {
					C.column = C.column + 4;
				}

				if(C.code_buffer[0] == '\0') {
					i = -1;
					C.token_start = C.column;
				}
			}
			else {
				// Other characters (Argument list, calculations, access, ...)
				switch(c) {
					// End of command / = / + / ! / < / >
					case (int)'=':
					case (int)'+':
					case (int)'!':
					case (int)'<':
					case (int)'>':
					case (int)';': {
						if(C.code_buffer[0] != '\0') {
							C.tokens[C.current_token_index].str = strdup(C.code_buffer);
							C.tokens[C.current_token_index].length = i + 1;
							C.tokens[C.current_token_index].column = C.token_start;
							C.tokens[C.current_token_index].line = C.line;
							C.token_start = C.column;
							C.current_token_index++;
							for(int j = 0;j < i;j++) {
								C.code_buffer[j] = '\0';
							}
						}
						C.code_buffer[0] = c;
						C.column++;
						C.tokens[C.current_token_index].str = strdup(C.code_buffer);
						C.tokens[C.current_token_index].length = 1;
						C.tokens[C.current_token_index].column = C.token_start;
						C.tokens[C.current_token_index].line = C.line;
						C.token_start = C.column;
						C.current_token_index++;
						C.code_buffer[0] = '\0';
						i = -1;
					} break;
					// Calculation / Caller Arg List
					case (int)'(':
					case (int)')': {
						if(C.code_buffer[0] != '\0') {
							C.tokens[C.current_token_index].str = strdup(C.code_buffer);
							C.tokens[C.current_token_index].length = i + 1;
							C.tokens[C.current_token_index].column = C.token_start;
							C.tokens[C.current_token_index].line = C.line;
							C.token_start = C.column;
							C.current_token_index++;
							for(int j = 0;j < i;j++) {
								C.code_buffer[j] = '\0';
							}
						}
						C.code_buffer[0] = c;
						C.column++;
						
						C.tokens[C.current_token_index].str = strdup(C.code_buffer);
						C.tokens[C.current_token_index].length = 1;
						C.tokens[C.current_token_index].column = C.token_start;
						C.tokens[C.current_token_index].line = C.line;
						C.token_start = C.column;
						C.current_token_index++;
						C.code_buffer[0] = '\0';
						i = -1;
					} break;
					// Code Object / Object
					case (int)'{':
					case (int)'}': {
						if(C.code_buffer[0] != '\0') {
							C.tokens[C.current_token_index].str = strdup(C.code_buffer);
							C.tokens[C.current_token_index].length = i + 1;
							C.tokens[C.current_token_index].column = C.token_start;
							C.tokens[C.current_token_index].line = C.line;
							C.token_start = C.column;
							C.current_token_index++;
							for(int j = 0;j < i;j++) {
								C.code_buffer[j] = '\0';
							}
						}
						
						C.code_buffer[0] = c;
						C.column++;
						
						C.tokens[C.current_token_index].str = strdup(C.code_buffer);
						C.tokens[C.current_token_index].length = 1;
						C.tokens[C.current_token_index].column = C.token_start;
						C.tokens[C.current_token_index].line = C.line;
						C.token_start = C.column;
						C.current_token_index++;
						C.code_buffer[0] = '\0';
						i = -1;
					} break;
					// Division / Comment
					case (int)'/': {
						if(C.code_buffer[0] != '\0') {
							C.tokens[C.current_token_index].str = strdup(C.code_buffer);
							C.tokens[C.current_token_index].length = i + 1;
							C.tokens[C.current_token_index].column = C.token_start;
							C.tokens[C.current_token_index].line = C.line;
							C.current_token_index++;
							for(int j = 0;j < i;j++) {
								C.code_buffer[j] = '\0';
							}
						}
						C.column++;
						c = fgetc(C.fptr);
						if(c == (int)'/') {
							while(c != (int)'\n' && c != EOF) {
								c = fgetc(C.fptr);
								if(c == (int)'\n') {
									C.line++;
									C.column = 1;
								}
							}
						}
						else if(c == (int)'*') {
							C.column++;
							while(c != EOF) {
								c = fgetc(C.fptr);
								if(c == (int)'*') {
									C.column++;
									c = fgetc(C.fptr);
									if(c == (int)'/') {
										C.column++;
										break;
									}
									else {
										C.column++;
									}
								}
								else if(c == (int)'\n') {
									C.line++;
									C.column = 1;
								}
								else if(c == (int)'\t') {
									C.column = C.column + 4;
								}
								else {
									C.column++;
								}
							}
						}
						C.token_start = C.column;
						i = -1;
					} break;
					default: {
						printf("[FATAL ERROR] Unsupported character, at %d:%d.\n", C.line, C.column);
						//return -1;
					} break;
				}
			}
			c = fgetc(C.fptr);
		}
		
		// Print tokens
		for(int j = 0;j < C.current_token_index;j++) {
			printf("\"");
			printf(C.tokens[j].str);
			printf("\", start: %d\n", C.tokens[j].column);
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
	
	// Clean up
	free(C.fName);
	free(C.ASSEMBLER);
	free(C.assembler_flags_length);
	free(C.assembler_flags);
	free(C.temp_assembly_file);
	for(int i = 0;i < C.current_token_index;i++) {
		free(C.tokens[i].str);
	}
	free(C.tokens);
	for(int i = 0;i < C.current_function;i++) {
		free(C.functions[i].name);
	}
	free(C.functions);
	for(int i = 0;i < C.current_identifier;i++) {
		free(C.identifiers[i].name);
	}
	free(C.identifiers);
	if(C.list_of_types != NULL) {
		free(C.list_of_types);
	}
	free(C.code_buffer);
	free(C.pre_compiled_code);
	return 0;
}