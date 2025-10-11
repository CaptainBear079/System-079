const int CODE_BUFFER_MAX_LENGTH = 256;

#if __32BIT__
	// 32-bit
	#define _BIT_RANGE int32_t
	#define U_BIT_RANGE uint32_t
#else
	// 64-bit
	#define _BIT_RANGE int64_t
	#define U_BIT_RANGE uint64_t
#endif

#define ERROR_IN_CURRENT if(info->error_in_current) {} \
	else {\
		printf("[ERROR] in file %s:\n", info->current_file->path);\
	}

#define FGETC_CHECK_EOF_WHILE(a, b) while(a) { \
		c = fgetc(b->current_file->fptr); \
		if(c == EOF) { \
			b->current_file->EOF_reached = true; \
			goto EOF_REACHED; \
		} \
		b->column++; \
	}

#define CHECK_IF_POINTER(a, b) { \
		int c = (int)' '; \
		while((char)c != '*') { \
			c = fgetc(info->current_file->fptr); \
			if(c == EOF) { \
				info->current_file->EOF_reached = true; \
				goto EOF_REACHED; \
			} \
			info->column++; \
			if((char)c == '*') { \
				t_operation.type = b; \
			} \
			else { \
			if(!(char)c == ' ') { \
				c = (int)' '; \
				FGETC_CHECK_EOF_WHILE((char)c == ' ', info) \
				if((char)c == '*') { \
					t_operation.type = b; \
				} \
				else { \
					t_operation.type = a; \
				} \
			} \
		} \
	} \
}

enum OPERATION_TYPES {
	CREATE__GLOBAL_INTEGER,
	CREATE__GLOBAL_CHARACTER,
	CREATE__GLOBAL_POINTER,
	CREATE__GLOBAL_VOID,
	CREATE__LOCAL_INTEGER,
	CREATE__LOCAL_CHARACTER,
	CREATE__LOCAL_POINTER,
	CREATE__LOCAL_VOID
};

enum IDENTIFIER_VALUE_STATES {
	IDENTIFIER_VALUE_UNDEFINED_OR_INVALID_SET,
	IDENTIFIER_VALUE_SET
};

typedef struct _File_ {
    FILE* fptr;
	char* path;
	bool EOF_reached;
} File;

typedef struct _IDENTIFIER_ {
	char** name; // Split into tokens
	int value_state;
	U_BIT_RANGE value;
} IDENTIFIER;

typedef struct _OPERATION_ {
	int type;               // Type of the operation
	IDENTIFIER* identifier; // Only when a identifier gets created
} OPERATION;

typedef struct _compiler_runtime_info_ {
	File* current_file;
	bool is_in_function;
	char* current_function;
	uint32_t line;
	uint32_t last_line;
	uint32_t column;
	uint32_t last_column;
	bool error_in_current;
} compiler_runtime_info;

int compile_file(compiler_runtime_info* info, File* file);