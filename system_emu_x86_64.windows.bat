TITLE System Emulator x86_64
REM This script builds the FAT32 boot image and runs it in QEMU.
START /wait nasm ^
  -f bin ^
  -o ./build/boot.bin ^
  ./bin/BOOT.asm
REM Create the boot image with a FAT32 filesystem
START qemu-system-x86_64 -drive file=./build/boot.bin,format=raw,if=ide