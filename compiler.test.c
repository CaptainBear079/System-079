#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum TOKEN_TYPES {
	TOKEN_TYPE__COMMAND_START,
	TOKEN_TYPE__COMMAND_START_KEYWORD_FAIL,
	TOKEN_TYPE__COMMAND_START_OPERATOR_FAIL,
	TOKEN_TYPE__COMMAND_START_CONSTANT_FAIL,
	TOKEN_TYPE__COMMAND_END,
	TOKEN_TYPE__CONST_UINT,
	TOKEN_TYPE__CONST_INT,
	TOKEN_TYPE__VAR_UINT,
	TOKEN_TYPE__VAR_INT,
	TOKEN_TYPE__ADD,
	TOKEN_TYPE__SET,
	TOKEN_TYPE__NEW_VAR_IDENTIFIER,
	TOKEN_TYPE__IDENTIFIER,
	TOKEN_TYPE__OPERATOR_SET,
	TOKEN_TYPE__OPERATOR_EQUALS,
	TOKEN_TYPE__FUNCTION
};

typedef struct _File_ {
    FILE* fptr;
	char* path;
} File;

typedef struct _Def_ {
    char* str;
} Def;

typedef struct _IDENTIFIER_ {
	char* name;
	bool set_by_malloc;
	char* type;
} IDENTIFIER;

typedef struct _TOKEN_ {
	uint32_t type;
	uint32_t value;
	void* ptr_value;
	IDENTIFIER* identifier;
} TOKEN;

typedef struct _PRE_INFO_ {
	char* title;
	int title_length;
	char* user;
	int user_length;
	char* main;
	int main_length;
	bool random;
	Def* def_req;
	int def_req_count;
} PRE_INFO;

// Arguments
#define ARG__DEFINE              (int)'D'       // Defines for pre-processing
#define ARG__EXECUTION_FORMAT    (int)'e'       // Sets the execution format (console or window or ...)
#define ARG__SET_FORMAT          (int)'f'       // Sets the format for the executable (syscalls, platform, file extension, ...)
#define ARG__WITH_DEBUG_INFO     (int)'g'       // Adds debug info
#define ARG__SET_OUTPUT          (int)'o'       // Sets the file used for the executable code

// Execution formats
#define ARG_EFORMAT__CONSOLE     "console"      // GUI Console                - Compatible (Standard)
#define ARG_EFORMAT__WINDOW      "window"       // GUI Window                 - Compatible
#define ARG_EFORMAT__BACKGROUND  "background"   // Background Process         - Compatible
#define ARG_EFORMAT__WEB         "web"          // Web application            - Compatible

// Formats
#define ARG_FORMAT__RAW16        "bin16"        // Raw binary 16 Bit          - Compatible
#define ARG_FORMAT__RAW32        "bin32"        // Raw binary 32 Bit          - Compatible
#define ARG_FORMAT__RAW64        "bin64"        // Raw binary 64 Bit          - Compatible
#define ARG_FORMAT__ELF32_RAW    "elf32_raw"    // Raw Elf 32 Bit             - Compatible
#define ARG_FORMAT__ELF32_LINUX  "elf32_linux"  // Linux 32 Bit syscalls      - Compatible
#define ARG_FORMAT__ELF32_SYS079 "elf32_sys079" // System 079 32 Bit syscalls - Optimized
#define ARG_FORMAT__ELF64_RAW    "elf_raw"      // Raw Elf                    - Compatible
#define ARG_FORMAT__ELF64_LINUX  "elf_linux"    // Linux syscalls             - Compatible (Standard)
#define ARG_FORMAT__ELF64_SYS079 "elf_sys079"   // System 079 syscalls        - Optimized
#define ARG_FORMAT__APK32        "apk32_sys079" // System 079 32 Bit syscalls - Optimized (Application Package Kill)
#define ARG_FORMAT__APK64        "apk64_sys079" // System 079 syscalls        - Optimized (Application Package Kill)
#define ARG_FORMAT__WIN32        "win32"        // Windows 32 Bit syscalls    - Barely compatible
#define ARG_FORMAT__WIN64        "win64"        // Windows 64 Bit syscalls    - Barely compatible

// Debug Formats
#define ARG_DEBUG__Z7            "Z7"           // /Z7 Debug info             - Compatible
#define ARG_DEBUG__Zi            "Zi"           // /Zi Debug info             - Compatible
#define ARG_DEBUG__ZI            "ZI"           // /ZI Debug info             - Compatible

// Execution format pre-processing defines
char* EFORMAT_DEF_1 = "__CONSOLE__                    \0";
char* EFORMAT_DEF_2 = "__WINDOW__                     \0";
char* EFORMAT_DEF_3 = "__BACKGROUND__                 \0";
char* EFORMAT_DEF_4 = "__WEB__                        \0";

// Format pre-processing defines
char* FORMAT_DEF_01 = "__RAW16__                      \0";
char* FORMAT_DEF_02 = "__RAW32__                      \0";
char* FORMAT_DEF_03 = "__RAW64__                      \0";
char* FORMAT_DEF_04 = "__ELF32_RAW__                  \0";
char* FORMAT_DEF_05 = "__ELF32_LINUX__                \0";
char* FORMAT_DEF_06 = "__ELF32_SYS079__               \0";
char* FORMAT_DEF_07 = "__ELF64_RAW__                  \0";
char* FORMAT_DEF_08 = "__ELF64_LINUX__                \0";
char* FORMAT_DEF_09 = "__ELF64_SYS079__               \0";
char* FORMAT_DEF_10 = "__APK32_SYS079__               \0";
char* FORMAT_DEF_11 = "__APK64_SYS079__               \0";
char* FORMAT_DEF_12 = "__WIN32__                      \0";
char* FORMAT_DEF_13 = "__WIN64__                      \0";

uint32_t max_defines_count;
uint32_t max_inst_count;
uint32_t max_input_files;

Def* Defines;
uint32_t defines_count;
TOKEN* inst;
uint32_t instruction_count;
IDENTIFIER* identifier;
uint32_t identifier_count;

char** preprocessed_files;

int preprocessor(File* finput, uint32_t input_File_count, PRE_INFO* PreInfo);
int compile(File* finput, uint32_t fcount);
int translate();
int assemble();
int Argument_processing__set_format(uint8_t* Format, char* argv);

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printf("[ERROR] Not enough arguments.\nNeed arguments: -f <Format> & <Path to input file>\n");
        return -1;
    }
    // Variables
    bool input_given          = true;
    uint32_t input_file_count = 0;
    File foutput;
    File* finput;
    uint8_t Format            = 7;
	uint8_t EFormat           = 0;
	uint8_t DFormat           = 0;
    
    if(strcmp(argv[1], "--var") == 0) {
        max_defines_count = atoi(argv[2]);
        max_inst_count    = atoi(argv[3]);
        max_input_files   = atoi(argv[4]);
    }
	else {
		max_defines_count = 500;
		max_inst_count    = 3000;
		max_input_files   = 100;
	}
	
	inst = malloc(max_inst_count * sizeof(TOKEN));

	identifier = malloc(1024 * sizeof(IDENTIFIER));

    char standardout[4096];  // Make sure this is large enough
    snprintf(standardout, sizeof(standardout), "%s/a.out", argv[0]);
    
    foutput.fptr = fopen(standardout, "wb");
    
    finput = malloc(max_input_files * sizeof(File));
    for(int i = max_input_files - 1; i > 0; i--) {
        finput[i].fptr = NULL;
    }
    
    defines_count = 0;
	Defines = malloc(max_defines_count * sizeof(Def));
    for(int i = 0; i < max_defines_count; i--) {
        Defines[i].str = malloc(256 * sizeof(char));
    }
    for(int i = 1; i < argc; i++) {
        char* str_p = argv[i];
        char c = str_p[0];
        if(c == '-') {
            c = str_p[1];
            switch((int)c) {
                case ARG__SET_OUTPUT: {
                    // Set output file
					i++;
                    fclose(foutput.fptr);
                    foutput.fptr = fopen(argv[i], "wb");
                    if(foutput.fptr == NULL) {
                        printf("[ERROR] Output file \"%s\" could not be found or created.\n", argv[i]);
                        return -1; // Chaos.ErrorMessages.ArgumentError
                    }
                } break;
                case ARG__DEFINE: {
                    // Add Define to pre-processing
					i++;
                    if(defines_count >= max_defines_count) {
                        printf("[ERROR] To many defines.\n");
                        return 1;
                    }
                    while(*argv[i]) {
                        int p = 0;
                        Defines[defines_count].str[p] = *argv[i];
                        argv[i]++;
                        p++;
                    }
                    defines_count++;
                } break;
                case ARG__SET_FORMAT: {
                    // Add Defines to pre-processing and set format
					i++;
                    int ret = 0;
                    ret = Argument_processing__set_format(&Format, argv[i]);
                    if(ret != 0) {
                        return ret;
                    }
                }
				case ARG__EXECUTION_FORMAT: {
					// Set execution format
					i++;
					char* def1 = NULL;
					if(strcmp(argv[i], ARG_EFORMAT__CONSOLE)) {
						EFormat = 0;
						def1 = EFORMAT_DEF_1;
					}
					else if(strcmp(argv[i], ARG_EFORMAT__WINDOW)) {
						EFormat = 1;
						def1 = EFORMAT_DEF_2;
					}
					else if(strcmp(argv[i], ARG_EFORMAT__BACKGROUND)) {
						EFormat = 2;
						def1 = EFORMAT_DEF_3;
					}
					else if(strcmp(argv[i], ARG_EFORMAT__WEB)) {
						EFormat = 3;
						def1 = EFORMAT_DEF_4;
					}
					else {
						printf("[ERROR] \"%s\" is an invalid execution format.", argv[i]);
						return -1; // Chaos.ErrorMessages.ArgumentError
					}
					Def Define;
					Define.str = malloc(256 * sizeof(Def));
					while(*def1) {
						int p = 0;
						Define.str[p] = def1[p];
						def1++;
						p++;
					}
					Defines[defines_count] = Define;
				} break;
				case ARG__WITH_DEBUG_INFO: {
					// Add debug info
					i++;
					if(argv[i] == ARG_DEBUG__Z7) {
						// Z7 format
						DFormat = 1;
					}
					else if(argv[i] == ARG_DEBUG__Zi) {
						// Zi format
						DFormat = 2;
					}
					else if(argv[i] == ARG_DEBUG__ZI) {
						// ZI format
						DFormat = 3;
					}
					else {
						// Invalid or unsupported debug info format
						printf("[ERROR] \"%s\" is an invalid or unsupported format for debug info.\n", argv[i]);
						return -1; // Chaos.ErrorMessages.ArgumentError
					}
				} break;
                default: {
                    // Invalid argument
                    printf("[ERROR] Argument \"%s\" is invalid.\n", argv[i]);
                    return -1; // Chaos.ErrorMessages.ArgumentError
                }
            }
        }
        else {
            // Add input file
            if(input_file_count >= max_input_files) {
                printf("[ERROR] To many input files.");
                return 1;
            }
			finput[input_file_count].path = malloc(strlen(argv[i]) * sizeof(char));
			strcpy(finput[input_file_count].path, argv[i]);
            finput[input_file_count].fptr = fopen(argv[i], "r");
            if(finput[input_file_count].fptr == NULL) {
                printf("[ERROR] Input file \"%s\" doesn't exist.", argv[i]);
                return -1; // Chaos.ErrorMessages.ArgumentError
            }
            input_given = true; // Input file for compiler
            input_file_count++;
        }
    }

	int ret;

	PRE_INFO PreInfo;
	PreInfo.title = malloc(128 * sizeof(char));
	strcpy(PreInfo.title, "Standard                                                                                                                     \0");
	PreInfo.def_req = malloc(32 * sizeof(Def));
	for(int i = 31; i > -1; i--) {
		PreInfo.def_req[i].str = malloc(128 * sizeof(char));
	}

	ret = preprocessor(finput, input_file_count, &PreInfo);
	if(ret != 0) {
		return ret;
	}

	ret = compile(finput, input_file_count);
	if(ret != 0) {
		return ret;
	}

	ret = translate();
	if(ret != 0) {
		return ret;
	}

	ret = assemble();
	if(ret != 0) {
		return ret;
	}

	// Free memory
	for(int i = input_file_count; i > 0; i--) {
		free(preprocessed_files[i]);
	}
	free(preprocessed_files);
	//free(Defines);
	free(inst);
	free(finput);

    return 0;
}

int preprocessor(File* finput, uint32_t input_File_count, PRE_INFO* PreInfo) {
	// Set "char** preprocessed_files" to array of char*
	preprocessed_files = malloc(input_File_count * sizeof(char*));
	// Loop trough all files
	for(int fi = 0; fi < input_File_count; fi++) {
		// Set a temporary FILE*
		FILE* t_fptr = finput[fi].fptr;

		// Get file length
		fseek(t_fptr, 0, SEEK_END);
		long file_length = ftell(t_fptr);
		fseek(t_fptr, 0, SEEK_SET);

		// Create entry in pre-processed files
		preprocessed_files[fi] = malloc((file_length + 1) * sizeof(char));
		preprocessed_files[fi][file_length] = '\0';

		int i = 0;
		int c = fgetc(t_fptr);
		while(c != EOF && i < file_length) {
			// Detect and handle pre-processor directives
			if((char)c == '#') {
				// Read directiv
				char t_c = (char)c;
				c = fgetc(t_fptr);
				if(c == EOF) {
					goto ERROR_PCK;
				}
				else if(isdigit((char)c)) {
					// Write the number in the pre-processed file
					preprocessed_files[fi][i] = t_c;
					i++;
					preprocessed_files[fi][i] = c;
					// Increment index (i) and read next char
					i++;
					c = fgetc(t_fptr);
				}
				else {
					switch(c) {
						case (int)'I': {
							// Directive only can be #INFO or an invalid
							c = fgetc(t_fptr);
							if(c == EOF) {
								goto ERROR_PCK;
							}
							switch(c) {
								case (int)'N': {
									c = fgetc(t_fptr);
									if(c == EOF) {
										goto ERROR_PCK;
									}
									switch(c) {
										case (int)'F': {
											c = fgetc(t_fptr);
											if(c == EOF) {
												goto ERROR_PCK;
											}
											switch(c) {
												case (int)'O': {

													// Handle #INFO directive
													char str[13];
													for(int i = 0; i < (128 + 1); i++) {
														c = fgetc(t_fptr);
														if(c == EOF) {
															goto ERROR_PCK;
														}
														str[i] = (char)c;
													}
													for(int i = 128 + 1; i < 128; i++) {
														str[i] = ' ';
													}
													str[128 + 1] = '\0';
													str[128] = '\0';
													if(strcmp(str, "TITLE=")) {
														char str2[128];
														c = fgetc(t_fptr);
														if(c == EOF) {
															goto ERROR_PCK;
														}
														if((char)c == '\"') {
															for(int i = 0; i < (128 + 1); i++) {
																c = fgetc(t_fptr);
																if(c == EOF) {
																	goto ERROR_PCK;
																}
																str2[i] = (char)c;
															}
															for(int i = 128 + 1; i < 128; i++) {
																str2[i] = ' ';
															}
															str2[128 + 1] = '\0';
															str2[128] = '\0';
														}
														else {
															for(int i = 0; i < 128; i++) {
																str2[i] = ' ';
															}
															str2[128 + 1] = '\0';
															str2[128] = '\0';
														}
														strcpy(PreInfo->title, str2);
													}
													for(int i = 0; i < (128 + 1); i++) {
														c = fgetc(t_fptr);
											if(c == EOF) {
												goto ERROR_PCK;
											}
														str[i] = (char)c;
													}
													for(int i = 128 + 1; i < 128; i++) {
														str[i] = ' ';
													}
													str[128 + 1] = '\0';
													str[128] = '\0';
													if(strcmp(str, "DEFINES_REQ")) {
														char str2[128];
														c = fgetc(t_fptr);
														if(c == EOF) {
															goto ERROR_PCK;
														}
														if((char)c == '\"') {
															for(int i = 0; i < (128 + 1); i++) {
																c = fgetc(t_fptr);
																if(c == EOF) {
																	goto ERROR_PCK;
																}
																str2[i] = (char)c;
															}
															for(int i = 128 + 1; i < 128; i++) {
																str2[i] = ' ';
															}
															str2[128 + 1] = '\0';
															str2[128] = '\0';
														}
														else {
															for(int i = 0; i < 128; i++) {
																str2[i] = ' ';
															}
															str2[128 + 1] = '\0';
															str2[128] = '\0';
														}
														strcpy(PreInfo->def_req[PreInfo->def_req_count].str, str2);
													}

												} break;
												default: {
													goto ERROR_PCK;
												} break;
											}
										} break;
										default: {
											goto ERROR_PCK;
										} break;
									}
								} break;
								default: {
									goto ERROR_PCK;
								} break;
							}
						} break;
						default: {
							ERROR_PCK:
							printf("[ERROR] Invalid or uncomplette pre-processor directiv.");
							return -2; // Problem between chair and keyboard
						} break;
					}
				}
			}
			else {
				// Write c to pre-processed file
				preprocessed_files[fi][i] = (char)c;
				// Increment index (i) and read next char
				i++;
				c = fgetc(t_fptr);
			}
		}
	}
}

int compile__PARSE_LOOP(
				uint32_t* t_instruction_count,
				char** code_buffer,
				TOKEN* t_token,
				TOKEN* last_token,
				int* _i,
				int* c,
				FILE** t_fptr
			) {
	PARSE_LOOP:
	// Read
	*c = fgetc(*t_fptr);
	PARSE_LOOP_WITHOUT_FETCH:
	if(*c == EOF) {
		if(*_i == 0) {
			return 0;
		}
	}
	else if((char)*c == '=') {
		// Set or equals
		*code_buffer[*_i] = (char)*c;
		*_i++;
		*c = fgetc(*t_fptr);
		if((char)*c == '=') {
			*code_buffer[*_i] = (char)*c;
			*_i++;
		}
		else {
			// Set
		}
	}
	*code_buffer[*_i] = '\0';
	*_i++;

	NO_LAST_TOKEN:
	// Check for EOF
	if (*c == EOF) {
		return 0;
	}
	// Parse keyword - WIP
	else if(last_token->type == TOKEN_TYPE__COMMAND_START) {
		if(strcmp(*code_buffer, "int") == 0) {
			t_token->type = TOKEN_TYPE__VAR_INT;
			t_token->identifier = &identifier[identifier_count];
			t_token->ptr_value = &inst[*t_instruction_count + 1];
			inst[*t_instruction_count] = *t_token;
			*last_token = *t_token;
			*_i = 0;
			goto PARSE_LOOP;
		}
	}
	// Parse Operators
	else if(last_token->type == TOKEN_TYPE__COMMAND_START_KEYWORD_FAIL || last_token->type == TOKEN_TYPE__NEW_VAR_IDENTIFIER) {
		if(strcmp(*code_buffer, ";") == 0) {
			t_token->type = TOKEN_TYPE__COMMAND_END;
			t_token->identifier = NULL;
			t_token->value = 0;
			inst[*t_instruction_count] = *t_token;
			*last_token = *t_token;
		}
		else if(strcmp(*code_buffer, "=") == 0) {
			t_token->type = TOKEN_TYPE__OPERATOR_SET;
			t_token->identifier = NULL;
			t_token->value = 0;
			inst[*t_instruction_count] = *t_token;
			*last_token = *t_token;
		}
		else if(strcmp(*code_buffer, "==") == 0) {
			t_token->type = TOKEN_TYPE__OPERATOR_EQUALS;
			t_token->identifier = NULL;
			t_token->value = 0;
			inst[*t_instruction_count] = *t_token;
			*last_token = *t_token;
		}
		else if(strcmp(*code_buffer, "+") == 0) {
			t_token->type = TOKEN_TYPE__ADD;
			t_token->identifier = NULL;
			t_token->value = 0;
			inst[*t_instruction_count] = *t_token;
			*last_token = *t_token;
		}
		else {
			t_token->type = TOKEN_TYPE__COMMAND_START_OPERATOR_FAIL;
			t_token->identifier = NULL;
			t_token->value = 0;
			inst[*t_instruction_count] = *t_token;
			*last_token = *t_token;
		}
	}
	// Parse constants - WIP
	else if(last_token->type == TOKEN_TYPE__COMMAND_START_OPERATOR_FAIL || last_token->type == TOKEN_TYPE__SET) {
		if(*code_buffer[0] == '0') {
			if(*code_buffer[1] == 'x') {
				int number = 0;
				for(int times = 16; *_i > 1; *_i--) {
					switch((int)*code_buffer[*_i]) {
						case (int)'0': {
							number = number + (0 * times);
						} break;
						case (int)'1': {
							number = number + (1 * times);
						} break;
						case (int)'2': {
							number = number + (2 * times);
						} break;
						case (int)'3': {
							number = number + (3 * times);
						} break;
						case (int)'4': {
							number = number + (4 * times);
						} break;
						case (int)'5': {
							number = number + (5 * times);
						} break;
						case (int)'6': {
							number = number + (6 * times);
						} break;
						case (int)'7': {
							number = number + (7 * times);
						} break;
						case (int)'8': {
							number = number + (8 * times);
						} break;
						case (int)'9': {
							number = number + (9 * times);
						} break;
						case (int)'A':
						case (int)'a': {
							number = number + (10 * times);
						} break;
						case (int)'B':
						case (int)'b': {
							number = number + (11 * times);
						} break;
						case (int)'C':
						case (int)'c': {
							number = number + (12 * times);
						} break;
						case (int)'D':
						case (int)'d': {
							number = number + (13 * times);
						} break;
						case (int)'E':
						case (int)'e': {
							number = number + (14 * times);
						} break;
						case (int)'F':
						case (int)'f': {
							number = number + (15 * times);
						} break;

						default: {
							return -2;
						} break;
					}
					times = times * 16;
				}
			}
		}
		else if(isdigit(*code_buffer[0])) {
			int number = 0;
			for(int times = 10; *_i > -1; *_i++) {
				number = number + (atoi(*code_buffer[*_i]) * times);
			}
		}
	}
	// Parse identifier - WIP
	else if(last_token->type == TOKEN_TYPE__COMMAND_START_CONSTANT_FAIL || last_token->type == TOKEN_TYPE__VAR_INT || TOKEN_TYPE__VAR_UINT) {
		identifier[identifier_count].name = malloc(*_i * sizeof(char));
		identifier[identifier_count].set_by_malloc = true;
		strcpy(identifier[identifier_count].name, *code_buffer);
		t_token->type = TOKEN_TYPE__NEW_VAR_IDENTIFIER;
		t_token->identifier = NULL;
		t_token->value = 0;
		inst[*t_instruction_count] = *t_token;
		*last_token = *t_token;
		*_i = 0;
		goto PARSE_LOOP;
	}
	else if(last_token->type == TOKEN_TYPE__COMMAND_END) {
		t_token->type = TOKEN_TYPE__COMMAND_START;
		t_token->identifier = NULL;
		t_token->value = 0;
		inst[*t_instruction_count] = *t_token;
		*last_token = *t_token;
		goto NO_LAST_TOKEN;
	}
	else {
		t_token->type = TOKEN_TYPE__COMMAND_START;
		t_token->identifier = NULL;
		t_token->value = 0;
		inst[*t_instruction_count] = *t_token;
		*last_token = *t_token;
		goto NO_LAST_TOKEN;
	}
}

int compile(File* finput, uint32_t input_File_count) {
	uint32_t t_instruction_count;
	uint8_t cb_pos = 0;
	char* code_buffer = malloc(256 * sizeof(char));
	char* code_temp_buffer = malloc(256 * sizeof(char));
	char* str_buffer = malloc(4096 * sizeof(char));
	TOKEN t_token;
	TOKEN last_token;
	char* buf;
	int _i = 0;
	int _i2 = 0;
	int c;
	for(int fi = 0; fi < input_File_count; fi++) {
		// Set temporary FILE*
		FILE* t_fptr = finput[fi].fptr;
			// Parse Code
			compile__PARSE_LOOP(&t_instruction_count, &code_buffer, &t_token, &last_token, &_i, &c, &t_fptr);
	}

	// Free memory
	free(code_buffer);
	free(code_temp_buffer);
	free(str_buffer);
	return 0;
}

int translate() {}

int assemble() {}

int Argument_processing__set_format(uint8_t* Format, char* argv) {
	if(defines_count >= 6000) {
		printf("[ERROR] To many defines.\n");
		return 1;
	}
	char* def1 = NULL;
	if(strcmp(argv, ARG_FORMAT__RAW16) == 0)             { *Format =  0; def1 = FORMAT_DEF_01; }
	else if(strcmp(argv, ARG_FORMAT__RAW32) == 0)        { *Format =  1; def1 = FORMAT_DEF_02; }
	else if(strcmp(argv, ARG_FORMAT__RAW64) == 0)        { *Format =  2; def1 = FORMAT_DEF_03; }
	else if(strcmp(argv, ARG_FORMAT__ELF32_RAW) == 0)    { *Format =  3; def1 = FORMAT_DEF_04; }
	else if(strcmp(argv, ARG_FORMAT__ELF32_LINUX) == 0)  { *Format =  4; def1 = FORMAT_DEF_05; }
	else if(strcmp(argv, ARG_FORMAT__ELF32_SYS079) == 0) { *Format =  5; def1 = FORMAT_DEF_06; }
	else if(strcmp(argv, ARG_FORMAT__ELF64_RAW) == 0)    { *Format =  6; def1 = FORMAT_DEF_07; }
	else if(strcmp(argv, ARG_FORMAT__ELF64_LINUX) == 0)  { *Format =  7; def1 = FORMAT_DEF_08; }
	else if(strcmp(argv, ARG_FORMAT__ELF64_SYS079) == 0) { *Format =  8; def1 = FORMAT_DEF_09; }
	else if(strcmp(argv, ARG_FORMAT__APK32) == 0)        { *Format =  9; def1 = FORMAT_DEF_10; }
	else if(strcmp(argv, ARG_FORMAT__APK64) == 0)        { *Format = 10; def1 = FORMAT_DEF_11; }
	else if(strcmp(argv, ARG_FORMAT__WIN32) == 0)        { *Format = 11; def1 = FORMAT_DEF_12; }
	else if(strcmp(argv, ARG_FORMAT__WIN64) == 0)        { *Format = 12; def1 = FORMAT_DEF_13; }
	else {
		printf("[ERROR] \"%s\" is an invalid Format.", argv);
		return -1; // Chaos.ErrorMessages.ArgumentError
	}

	// Add the format #define
	strncpy(Defines[defines_count].str, def1, 255);
	Defines[defines_count].str[255] = '\0';
	defines_count++;

	return 0; // Success
}
