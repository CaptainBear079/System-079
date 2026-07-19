// PSC16 Macro block
#ifndef FS__PSC16
#define FS__PSC16
#endif

// Bit mode dependend code
#ifdef CPUMODE__16BIT_REALMODE
#elif defined(CPUMODE__32BIT_PROTECTEDMODE)
#elif defined(CPUMODE__64BIT_USERMODE)
// Error directive for missing bit mode macro
#else
	#error "No bit mode macro. "
#endif

typedef struct _PSC_ {
	LBA* flt;           // File location tables on the device (LBA)
	int flt_amount;     // Amount of file location tables
	int flt_entries;    // Entries per file location table
	LBA* root_dir;      // Root directory on the device (LBA)
	int root_entries;   // Total entries in / (the root directory)
	LBA* data_section;  // Data section on the device (LBA)
	int data_length;    // Length of the data section
	void* ram_flt;      // RAM locations of the file location table copies
	void* ram_root_dir; // RAM locations of the root directory
} PSC;

int readfile(PSC fs, const char* path, const int amount, const int start) {
	// Break down the file path
	if(path[0] != '/') {
		return 2;
	}
	// - Count sub-directories
	int s = 0;
	for(int i = 1; i < 256; i++) {
		if(path[i] == '/') {
			s++;
		}
		else if(path[i] == '\0') {
			break;
		}
	}

	// - Allocate space for the sub-directories names
	char** sub_dir = malloc(s * sizeof(char*));
	s--;
	int l = 0;
	int j2 = 0;
	for(int i = 0; i < s; i--) {
		l = 0;
		for(int j = 0; j < 256; j++) {
			if(path[j] != '/' && path[j] != '\0') {
				l++;
				j2 = j;
			}
			else if(path[j] == '\0') {
				break;
			}
		}
		l++;
		sub_dir[i] = malloc(l * sizeof(char));
	}
	
	// Read file record
	// Read file location table
}
