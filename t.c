#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "t.h"

int get_token(compiler_runtime_info* info, char* buffer, int* c, int max_length);
int check_identifier(compiler_runtime_info* info, int* c);
int read_calculation(compiler_runtime_info* info, int* c, OPERATION* t_operation, char end_char);

int TAB_WIDTH;

int compile_file(compiler_runtime_info* info, File* file) {
	TAB_WIDTH = 4;
	info->current_file = file;
	info->is_in_function = false;
	info->current_function = malloc(256 * sizeof(char));
	strncpy(info->current_function, " ", 255);
	char* code_buffer = malloc(CODE_BUFFER_MAX_LENGTH * sizeof(char));
	char* buffer = malloc(CODE_BUFFER_MAX_LENGTH * sizeof(char));
	// Stop on EOF
	while(!info->current_file->EOF_reached) {
		// Get token
		{
			int c = (int)' ';
			get_token(info, code_buffer, &c, CODE_BUFFER_MAX_LENGTH);
		}
		// Check if is in function
		if(!info->is_in_function) {
			// Outside a function
			if(strncmp(code_buffer, "//", 2) == 0) {
				// Full-Line Comment
				{
					int c = (int)' ';
					FGETC_CHECK_EOF_WHILE((char)c != '\n', info)
					info->line++;
					info->column = 1;
				}
			}
			else if(strncmp(code_buffer, "/*", 2) == 0) {
				// Sectional Comment
				{
					bool done = false;
					int c = (int)' ';
					while(!done) {
						c = fgetc(info->current_file->fptr);
						if(c == EOF) {
							info->current_file->EOF_reached = true;
							goto EOF_REACHED;
						}
						info->column++;
						if((char)c == '\n') {
							info->line++;
							info->column = 1;
						}
						if((char)c == '*') {
							c = fgetc(info->current_file->fptr);
							if(c == EOF) {
								info->current_file->EOF_reached = true;
								goto EOF_REACHED;
							}
							info->column++;
							if((char)c == '/') {
								done = true;
							}
						}
					}
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
				printf("at line %d, column %d: \"%s\" is not a valid type, function return type or comment. -[SyntaxError_TYPO?]", info->line, info->column, code_buffer);
			}
		}
		else {
			// Inside a function
			if(strncmp(code_buffer, "//", 2) == 0) {
				// Full-Line Comment
				{
					int c = (int)' ';
					FGETC_CHECK_EOF_WHILE((char)c != '\n', info)
					info->line++;
					info->column = 1;
				}
			}
			else if(strncmp(code_buffer, "/*", 2) == 0) {
				// Sectional Comment
				{
					bool done = false;
					int c = (int)' ';
					while(!done) {
						c = fgetc(info->current_file->fptr);
						if(c == EOF) {
							info->current_file->EOF_reached = true;
							goto EOF_REACHED;
						}
						info->column++;
						if((char)c == '\n') {
							info->line++;
							info->column = 1;
						}
						if((char)c == '*') {
							c = fgetc(info->current_file->fptr);
							if(c == EOF) {
								info->current_file->EOF_reached = true;
								goto EOF_REACHED;
							}
							info->column++;
							if((char)c == '/') {
								done = true;
							}
						}
					}
				}
			}
			else if(strcmp(code_buffer, "int") == 0) {
				// Local integer
				IDENTIFIER t_identifier = { NULL, 0, 0 };
				OPERATION t_operation = { CREATE__LOCAL_INTEGER, &t_identifier };

				// Get name
				t_operation.identifier->name = malloc(10 * sizeof(char*));
				for(int i, len = 0; len < 255; i++) {
					int c = (int)' ';
					len = get_token(info, buffer, &c, CODE_BUFFER_MAX_LENGTH);
					strcpy(t_operation.identifier->name[i], buffer);
				}
			}
			else if(strcmp(code_buffer, "char") == 0) {
				// Local character
				IDENTIFIER t_identifier = { NULL, 0, 0 };
				OPERATION t_operation = { CREATE__LOCAL_CHARACTER, &t_identifier };
				
				// Get name
				t_operation.identifier->name = malloc(10 * sizeof(char*));
				for(int i, len = 0; len < 255; i++) {
					int c = (int)' ';
					len = get_token(info, buffer, &c, CODE_BUFFER_MAX_LENGTH);
					strcpy(t_operation.identifier->name[i], buffer);
				}
			}
			else if(strcmp(code_buffer, "void") == 0) {
				// Local pointer or void (Void is not a type)
				// Check if pointer or void
				IDENTIFIER t_identifier = { NULL, 0, 0 };
				OPERATION t_operation = { -1, &t_identifier };
				CHECK_IF_POINTER(CREATE__LOCAL_VOID, CREATE__LOCAL_POINTER)

				// Get name
				t_operation.identifier->name = malloc(10 * sizeof(char*));
				for(int i, len = 0; len < 255; i++) {
					int c = (int)' ';
					len = get_token(info, buffer, &c, CODE_BUFFER_MAX_LENGTH);
					strcpy(t_operation.identifier->name[i], buffer);
				}

				// Set value
				{
					int c = (int)' ';
					FGETC_CHECK_EOF_WHILE((char)c != ';' || (char)c != '=', info)
					if((char)c == ';') {
						// Variable standard value
						t_operation.identifier->value_state = IDENTIFIER_VALUE_UNDEFINED_OR_INVALID_SET;
					}
					if((char)c == '=') {
						// Variable set address
						c = (int)' ';
						while((char)c == ' ') {
							c = fgetc(info->current_file->fptr);
							if(c == EOF) {
								info->current_file->EOF_reached = true;
								printf("at line %d, column %d: Unfinished pointer change without termination by \';\' inside a function. -[Warning_UnfinishedCode]", info->line, info->column);
								goto EOF_REACHED;
							}
							info->column++;
						}
						// Get address
						if((char)c == '&') {
							check_identifier(info, &c);
						}
						else if((char)c == '0') {
							// Hexadecimal or Binary address
							c = fgetc(info->current_file->fptr);
							if(c == EOF) {
								info->current_file->EOF_reached = true;
								printf("at line %d, column %d: Unfinished pointer change without termination by \';\' inside a function. -[Warning_UnfinishedCode]", info->line, info->column);
								goto EOF_REACHED;
							}
							info->column++;
						}
						// Check if a arithmetic calculation is being done, to change the address as example for offsets (addition, subtraction, multiplication, division, modulus)
						if((char)c == ' ') {
							c = (int)' ';
							while((char)c == ' ') {
								c = fgetc(info->current_file->fptr);
								if(c == EOF) {
									info->current_file->EOF_reached = true;
									goto EOF_REACHED;
								}
								info->column++;
							}
							if((char)c == '+') {
								// Addition
								read_calculation(info, &c, &t_operation, ';');
							}
							else {
								// Invalid: No address given
								ERROR_IN_CURRENT
								printf("at line %d, column %d: No identifier name given after '&'. -[SyntaxError_ADDRESS_OPERATOR]", info->line, info->column);
								t_operation.identifier->value_state = IDENTIFIER_VALUE_UNDEFINED_OR_INVALID_SET;
							}
						}
					}
				}
			}
			else {
				ERROR_IN_CURRENT
				printf("at line %d, column %d: \"%s\" is not a valid type or comment. -[SyntaxError_TYPO?]", info->line, info->column, code_buffer);
			}
		}
		EOF_REACHED:
	}
}

int get_token(compiler_runtime_info* info, char* buffer, int* c, int max_length) {
	// Find first character that is not a space, tab or newline
	while((char)*c == ' ' || (char)*c == '\t' || (char)*c == '\n') {
		*c = fgetc(info->current_file->fptr);
		if(*c == EOF) {
			info->current_file->EOF_reached = true;
			return -1;
		}
		if((char)*c == ' ') {
			info->column++;
		}
		else if((char)*c == '\t') {
			info->column + TAB_WIDTH;
		}
		else if((char)*c == '\n') {
			info->line++;
			info->column = 1;
		}
	}
	for(int i = 0; i <= max_length; i++) {
		if((char)*c == ' ' || (char)*c == '\t' || (char)*c == '\n') {
			buffer[i] = '\0';
			return strlen(buffer);
		}
		else if(!isalnum((char)*c)) {
			buffer[i] = '\0';
			return strlen(buffer);
		}
		buffer[i] = (char)*c;
		i++;
		*c = fgetc(info->current_file->fptr);
		if(*c == EOF) {
			info->current_file->EOF_reached = true;
			buffer[i] = '\0';
			return strlen(buffer);
		}
	}
	return strlen(buffer);
}

int check_identifier(compiler_runtime_info* info, int* c) {}

int read_calculation(compiler_runtime_info* info, int* c, OPERATION* t_operation, char end_char) {}