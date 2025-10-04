#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define CODE_BUFFER_MAX_LENGTH 256

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

typedef struct _File_ {
    FILE* fptr;
	char* path;
	bool EOF_reached;
} File;

typedef struct _TOKEN_ {
	int type;
	char name[256];
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

int main() {
	compiler_runtime_info info = { false, "" };
	char* code_buffer = malloc(CODE_BUFFER_MAX_LENGTH * sizeof(char));
	char* last_code_buffer = malloc(CODE_BUFFER_MAX_LENGTH * sizeof(char));
	// Check if is in function
	if(!info.is_in_function) {
		// Outside a function
		if(strncmp(code_buffer, "//", 2) == 0) {
			// Full-Line Comment
			{
				int c = (int)' ';
				while((char)c != '\n' || !info.current_file->EOF_reached) {
					c = fgetc(info.current_file->fptr);
					if(c == EOF) {
						info.current_file->EOF_reached = true;
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
				while(!done || !info.current_file->EOF_reached) {
					c = fgetc(info.current_file->fptr);
					if(c == EOF) {
						info.current_file->EOF_reached = true;
					}
					info.column++;
					if((char)c == '\n') {
						info.line++;
						info.column = 0;
					}
					if((char)c == '*') {
						c = fgetc(info.current_file->fptr);
						info.column++;
						if((char)c == '/') {
							done = true;
						}
					}
				}
			}
		}
		else if(strcmp(code_buffer, "system") == 0) {
			get_token();
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
				printf("at line %d, column %d: \"%s %s\" is not a valid type, function return type or comment. -[SyntaxError]", info.line, info.column, last_code_buffer, code_buffer);
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
				while((char)c != '\n' || !info.current_file->EOF_reached) {
					c = fgetc(info.current_file->fptr);
					if(c == EOF) {
						info.current_file->EOF_reached = true;
					}
				}
				info.line++;
				info.column = info.last_column;
			}
		}
		else if(strncmp(code_buffer, "/*", 2) == 0) {
			// Sectional Comment
			{
				bool done = false;
				int c = (int)' ';
				while(!done || !info.current_file->EOF_reached) {
					c = fgetc(info.current_file->fptr);
					if(c == EOF) {
						info.current_file->EOF_reached = true;
					}
					info.column++;
					if((char)c == '\n') {
						info.line++;
						info.column = 0;
					}
					if((char)c == '*') {
						c = fgetc(info.current_file->fptr);
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
			strncpy(t_token.name, get_token(), 255);
			t_token.name[255] = '\0';
		}
		else if(strcmp(code_buffer, "char") == 0) {
			// Local character
			TOKEN t_token = { LOCAL_CHARACTER, "" };
			strncpy(t_token.name, get_token(), 255);
			t_token.name[255] = '\0';
		}
		else if(strcmp(code_buffer, "void") == 0) {
			// Local pointer
			TOKEN t_token = { -1, "" };
			{
				int c = (int)' ';
				while((char)c != '*' || !info.current_file->EOF_reached) {
					c = fgetc(info.current_file->fptr);
					if(c == EOF) {
						info.current_file->EOF_reached = true;
					}
					info.column++;
					if((char)c == '*') {
						t_token.type = LOCAL_POINTER;
					}
					else {
						if(!(char)c == ' ') {
							while(c == ' ') {
								c = fgetc(info.current_file->fptr);
								if(c == EOF || !info.current_file->EOF_reached) {
									info.current_file->EOF_reached = true;
								}
							}
							t_token.type = LOCAL_VOID;
						}
					}
				}
			}
			strncpy(t_token.name, get_token(), 255);
			t_token.name[255] = '\0';
		}
		else {
			ERROR_IN_CURRENT
			printf("at line %d, column %d: \"%s\" is not a valid type or comment. -[SyntaxError]", info.line, info.column, code_buffer);
		}
	}
}