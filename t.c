#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define CODE_BUFFER_MAX_LENGTH 256

#if __32BIT__
	// 32-bit
	#define _BIT_RANGE int32_t
	#define U_BIT_RANGE uint32_t
#else
	// 64-bit
	#define _BIT_RANGE int64_t
	#define U_BIT_RANGE uint64_t
#endif

#define ERROR_IN_CURRENT if(info.error_in_current) {} \
	else {\
		printf("[ERROR] in file %s:\n", info.current_file->path);\
	}

enum TOKEN_TYPES {
	GLOBAL_INTEGER,
	GLOBAL_CHARACTER,
	GLOBAL_POINTER,
	GLOBAL_VOID,
	LOCAL_INTEGER,
	LOCAL_CHARACTER,
	LOCAL_POINTER,
	LOCAL_VOID
};

enum TOKEN_VALUE_STATES {
	TOKEN_VALUE_UNDEFINED_OR_INVALID_SET,
	TOKEN_VALUE_SET
};

typedef struct _File_ {
    FILE* fptr;
	char* path;
	bool EOF_reached;
} File;

typedef struct _TOKEN_ {
	int type;
	char name[256];
	int value_state;
	U_BIT_RANGE value;
} TOKEN;

typedef struct _compiler_runtime_info_ {
	File* current_file;
	bool is_in_function;
	char current_function[256];
	uint32_t line;
	uint32_t last_line;
	uint32_t column;
	uint32_t last_column;
	bool error_in_current;
} compiler_runtime_info;

int compile_file() {
	compiler_runtime_info info = { false, "" };
	char* code_buffer = malloc(CODE_BUFFER_MAX_LENGTH * sizeof(char));
	char* buffer = malloc(CODE_BUFFER_MAX_LENGTH * sizeof(char));
	// Stop on EOF
	while(!info.current_file->EOF_reached) {
		// Get token
		get_token(&info, code_buffer, CODE_BUFFER_MAX_LENGTH);
		// Check if is in function
		if(!info.is_in_function) {
			// Outside a function
			if(strncmp(code_buffer, "//", 2) == 0) {
				// Full-Line Comment
				{
					int c = (int)' ';
					while((char)c != '\n') {
						c = fgetc(info.current_file->fptr);
						if(c == EOF) {
							info.current_file->EOF_reached = true;
							goto EOF_REACHED;
						}
					}
					info.line++;
					info.column = 1;
				}
			}
			else if(strncmp(code_buffer, "/*", 2) == 0) {
				// Sectional Comment
				{
					bool done = false;
					int c = (int)' ';
					while(!done) {
						c = fgetc(info.current_file->fptr);
						if(c == EOF) {
							info.current_file->EOF_reached = true;
							goto EOF_REACHED;
						}
						info.column++;
						if((char)c == '\n') {
							info.line++;
							info.column = 1;
						}
						if((char)c == '*') {
							c = fgetc(info.current_file->fptr);
							if(c == EOF) {
								info.current_file->EOF_reached = true;
								goto EOF_REACHED;
							}
							info.column++;
							if((char)c == '/') {
								done = true;
							}
						}
					}
				}
			}
			else if(strcmp(code_buffer, "system") == 0) {
				get_token(&info, buffer, CODE_BUFFER_MAX_LENGTH);
				if(strcmp(code_buffer, "int") == 0) {
					// System type integer return
				}
				else if(strcmp(code_buffer, "char") == 0) {
					// System type character return
				}
				else if(strcmp(code_buffer, "void") == 0) {
					// System type pointer return
				}
				else {
					ERROR_IN_CURRENT
					printf("at line %d, column %d: \"%s\" is not a valid type, function return type or comment. -[SyntaxError]", info.line, info.column, code_buffer);
				}
			}
			else if(strcmp(code_buffer, "int") == 0) {
				// Global integer or integer return function
			}
			else if(strcmp(code_buffer, "char") == 0) {
				// Global character or character return function
			}
			else if(strcmp(code_buffer, "void") == 0) {
				// Global pointer or pointer return function
			}
			else {
				ERROR_IN_CURRENT
				printf("at line %d, column %d: \"%s\" is not a valid type, function return type or comment. -[SyntaxError]", info.line, info.column, code_buffer);
			}
		}
		else {
			// Inside a function
			if(strncmp(code_buffer, "//", 2) == 0) {
				// Full-Line Comment
				{
					int c = (int)' ';
					while((char)c != '\n') {
						c = fgetc(info.current_file->fptr);
						if(c == EOF) {
							info.current_file->EOF_reached = true;
							goto EOF_REACHED;
						}
					}
					info.line++;
					info.column = 1;
				}
			}
			else if(strncmp(code_buffer, "/*", 2) == 0) {
				// Sectional Comment
				{
					bool done = false;
					int c = (int)' ';
					while(!done) {
						c = fgetc(info.current_file->fptr);
						if(c == EOF) {
							info.current_file->EOF_reached = true;
							goto EOF_REACHED;
						}
						info.column++;
						if((char)c == '\n') {
							info.line++;
							info.column = 1;
						}
						if((char)c == '*') {
							c = fgetc(info.current_file->fptr);
							if(c == EOF) {
								info.current_file->EOF_reached = true;
								goto EOF_REACHED;
							}
							info.column++;
							if((char)c == '/') {
								done = true;
							}
						}
					}
				}
			}
			else if(strcmp(code_buffer, "int") == 0) {
				// Local integer
				TOKEN t_token = { LOCAL_INTEGER, "" };
				char* t = get_token(&info, buffer, CODE_BUFFER_MAX_LENGTH);
				strncpy(t_token.name, t, 255);
				t_token.name[255] = '\0';
			}
			else if(strcmp(code_buffer, "char") == 0) {
				// Local character
				TOKEN t_token = { LOCAL_CHARACTER, "" };
				char* t = get_token(&info, buffer, CODE_BUFFER_MAX_LENGTH);
				strncpy(t_token.name, t, 255);
				t_token.name[255] = '\0';
			}
			else if(strcmp(code_buffer, "void") == 0) {
				// Local pointer or void (Void is not a type)
				// Check if pointer or void
				TOKEN t_token = { -1, "" };
				{
					int c = (int)' ';
					while((char)c != '*') {
						c = fgetc(info.current_file->fptr);
						if(c == EOF) {
							info.current_file->EOF_reached = true;
							goto EOF_REACHED;
						}
						info.column++;
						if((char)c == '*') {
							t_token.type = LOCAL_POINTER;
						}
						else {
							if(!(char)c == ' ') {
								c = (int)' ';
								while(c == ' ') {
									c = fgetc(info.current_file->fptr);
									if(c == EOF) {
										info.current_file->EOF_reached = true;
										goto EOF_REACHED;
									}
									info.column++;
								}
								if((char)c == '*') {
									t_token.type = LOCAL_POINTER;
								}
								else {
									t_token.type = LOCAL_VOID;
								}
							}
						}
					}
				}

				// Get name
				char* t = get_token(&info, buffer, CODE_BUFFER_MAX_LENGTH);
				strncpy(t_token.name, t, 255);
				t_token.name[255] = '\0';

				// Set value
				{
					int c = (int)' ';
					while((char)c != ';' || (char)c != '=') {
						c = fgetc(info.current_file->fptr);
						if(c == EOF) {
							info.current_file->EOF_reached = true;
							goto EOF_REACHED;
						}
						info.column++;
					}
					if((char)c == ';') {
						// Variable standard value
						t_token.value_state = TOKEN_VALUE_UNDEFINED_OR_INVALID_SET;
					}
					if((char)c == '=') {
						// Variable set address
						c = (int)' ';
						while((char)c == ' ') {
							c = fgetc(info.current_file->fptr);
							if(c == EOF) {
								info.current_file->EOF_reached = true;
								goto EOF_REACHED;
							}
							info.column++;
						}
						// Get address
						if((char)c == '&') {
							check_identifier(info, &c);
							// Check if a arithmetic calculation is being done, to change the address as example for offsets (addition, subtraction, multiplication, division, modulus)
							if((char)c == ' ') {
								c = (int)' ';
								while((char)c == ' ') {
									c = fgetc(info.current_file->fptr);
									if(c == EOF) {
										info.current_file->EOF_reached = true;
										goto EOF_REACHED;
									}
									info.column++;
								}
								if((char)c == '+') {
									// Addition
									read_calculation(info, &c, &t_token, ';');
								}
								else {
									// Invalid: No address given
									ERROR_IN_CURRENT
									printf("at line %d, column %d: No identifier name given after '&'. -[SyntaxError]", info.line, info.column);
									t_token.value_state = TOKEN_VALUE_UNDEFINED_OR_INVALID_SET;
								}
							}
						}
					}
				}
			}
			else {
				ERROR_IN_CURRENT
				printf("at line %d, column %d: \"%s\" is not a valid type or comment. -[SyntaxError]", info.line, info.column, code_buffer);
			}
		}
		EOF_REACHED:
	}
}

char* get_token(compiler_runtime_info* info, char* buffer, int max_length) {
	int i = 0;
	int c = (int)' ';

	// Find first character that is not a space, tab or newline
	while((char)c == ' ' || (char)c == '\t' || (char)c == '\n') {
		c = fgetc(info->current_file->fptr);
		if(c == EOF) {
			info->current_file->EOF_reached = true;
			return NULL;
		}
	}
}