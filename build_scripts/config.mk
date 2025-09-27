
BUILD_DIR = $(abspath ./build)
BOOTLOADER_STAGE1 = $(abspath ./dev/src/bootloader/stage1)
BOOTLOADER_STAGE2 = $(abspath ./dev/src/bootloader/stage2)
KERNEL = $(abspath ./dev/src/kernel)

TOOLCHAIN_PREFIX = $(abspath ./../toolchain/$(TOOLCHAIN))
TOOLCHAIN_TARGET = $(TOOLCHAIN)

BOOTLOADER_STAGE1_BIN = bootloader.bin

BOOTLOADER_STAGE2_BIN = bootmanager.bin
STAGE2_ENTRY = entry.asm
STAGE2_REALMODE = /x86_assembly/realmode.asm
STAGE2_PROTECTEDMODE = /x86_assembly/protectedmode.asm

KERNEL_BIN = kernel.bin

export PATH := $(TOOLCHAIN_PREFIX)/bin:$(PATH)

export TOOLCHAIN = i686-elf

export CC = gcc
export CXX = g++
export LINKER = gcc
export CFLAGS = -c -Wall -std=c99 -g -fno-builtin-printf
export CXXFLAGS =
export LINKER_FLAGS = -x none -nostdlib
export ASM = /usr/bin/nasm
export ASMFLAGS = -f elf64

export TARGET_CC = $(TOOLCHAIN_PREFIX)/bin/$(TOOLCHAIN)-gcc
export TARGET_CXX = $(TOOLCHAIN_PREFIX)/bin/$(TOOLCHAIN)-g++
export TARGET_LINKER = $(TOOLCHAIN_PREFIX)/bin/$(TOOLCHAIN)-gcc
export TARGET_CFLAGS = -c -Wall -std=c99 -g -nostdlib -ffreestanding
export TARGET_CXXFLAGS =
export TARGET_LINKER_FLAGS = -nostdlib -ffreestanding -lgcc
export TARGET_ASM = /usr/bin/nasm
export TARGET_ASMFLAGS = -f elf
export TARGET_STAGE1_ASMFLAGS = -f bin

export EMULATOR = qemu-system-x86_64
export EMULATOR_FLAGS = -drive format=raw,file=$(BUILD_DIR)/stage2-bootmanager.img -m 512M -smp 2
