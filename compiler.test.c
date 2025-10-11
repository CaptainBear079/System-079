#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "t.h"

typedef struct _Def_ {
    char* str;
} Def;

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
OPERATION* inst;
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
	
	inst = malloc(max_inst_count * sizeof(OPERATION));

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
                } break;
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

int compile(File* finput, uint32_t input_File_count) {
	compiler_runtime_info info;
	for(int fi = 0; fi < input_File_count; fi++) {
		// Set temporary FILE*
			// Parse Code
			compile_file(&info, &finput[fi]);
	}
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
