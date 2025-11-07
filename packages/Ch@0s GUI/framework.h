#ifdef __ELF32_SYS079__
#include <sys079/process.h>
#else
#ifdef __APK32_SYS079__
#include <sys079/process.h>
#else
#ifdef __ELF64_SYS079__
#include <sys079/process.h>
#else
#ifdef __APK64_SYS079__
#include <sys079/process.h>
#else
#ifdef __ELF32_LINUX__
#include <elf32_linux/process.h>
#else
#ifdef __ELF64_LINUX__
#include <elf64_linux/process.h>
#else
#ifdef __WIN32__
#include <win32/process.h>
#else
#ifdef __WIN64__
#include <win64/process.h>
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
