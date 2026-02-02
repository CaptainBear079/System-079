# Introduction
In this project we are building an operating system and AI from scratch. We optimize the OS and tools for privacy, AI and robotic use.

### Please read the LICENSE file and installation instruction section. By downloading or any kind of copying you agree with the license, it's conditions and that you know what you are doing.

# Features
Operating System:
- Bootloader stage two loading (raw binary).
- Kernel binary loading (raw binary, no system stuff yet).
- Compiler for custom C-like programming language (ChaosLang).

# Coming up
- Basic text based kernel
- Basic file system
- First compiler release
- Basic AI tools

# Tools
For development used tools are:
- High level programming:
  - [GCC](https://gcc.gnu.org/) - GNU Compiler Collection
  - [G++](https://gcc.gnu.org/) - GNU C++ Compiler (included in GCC)
  - ChaosLang - Our custom programming language (high level modules)
- Low level programming:
  - [Make](https://www.gnu.org/software/make/) - Build automation tool (for GCC toolchain and automated building)
  - [GCC](https://gcc.gnu.org/) - GNU Compiler Collection custom built for cross compiling
  - [G++](https://gcc.gnu.org/) - GNU C++ Compiler custom built for cross compiling (included in GCC)
  - ChaosLang - Our custom programming language (low level modules)
  - [NASM](https://www.nasm.us/) - Netwide Assembler
  - A virtual machine of your choice
  - A virtual machine with debugger (like [Bochs](https://bochs.sourceforge.io/))

# Installation instruction
If you want to build it yourself please go to the section "Building from source".

## OS Installation
1. Download the latest release.
2. Burn the ISO image to a USB drive, SSD/HDD or DVD.
3. Plug in the installation media and restart your computer (you might need to change your boot order).
4. Follow the instructions to complete the installation.

## Installing our custom tools
1. Download the latest release for your tool.
2. Start the installer and follow the instructions.
3. After installation, you might need to add the tool's installation directory to your system's PATH environment variable.

# Building from source
1. Download and unpack the source code from the release page.
2. Make sure you have dependencies installed and toolchain built (see Tools section and the release sites dependency list).
3. Open a terminal and navigate to the project directory.
4. Run the build script "build.sh" (Linux/MacOS) or "build.bat" (Windows).
5. Run the make command for the tool your trying to build (see release page).
6. After the build process is complete, you can find the compiled binaries in the "build" directory.