BINUTILS_VERSION = 2.45
BINUTILS_URL = https://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VERSION).tar.xz
BINUTILS_BUILD = $(abspath ./../../../toolchain/binutils-build-$(BINUTILS_VERSION))

GCC_VERSION = 15.2.0
GCC_URL = https://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.xz
GCC_BUILD = $(abspath ./../../../toolchain/gcc-build-$(GCC_VERSION))

TOOLCHAIN_PREFIX = $(abspath ./../../../toolchain/$(TOOLCHAIN))
TOOLCHAIN_TARGET = i686-elf

build-toolchain: build-binutils build-gcc

build-binutils: get-binutils
	mkdir -p $(BINUTILS_BUILD)
	cd $(BINUTILS_BUILD) && ../binutils-$(BINUTILS_VERSION)/configure \
		--prefix=$(TOOLCHAIN_PREFIX) \
		--target=$(TOOLCHAIN_TARGET) \
		--with-sysroot \
		--disable-nls \
		--disable-werror
	$(MAKE) -j 4 -C $(BINUTILS_BUILD)
	$(MAKE) -C $(BINUTILS_BUILD) install

get-binutils:
	mkdir -p ./../../../toolchain
	cd ./../../../toolchain && wget $(BINUTILS_URL) && tar -xvf binutils-$(BINUTILS_VERSION).tar.xz

build-gcc: build-binutils get-gcc
	mkdir -p $(GCC_BUILD)
	cd $(GCC_BUILD) && ../gcc-$(GCC_VERSION)/configure \
		--prefix=$(TOOLCHAIN_PREFIX) \
		--target=$(TOOLCHAIN_TARGET) \
		--disable-nls \
		--enable-languages=c,c++ \
		--without-headers
	$(MAKE) -j 4 -C $(GCC_BUILD) all-gcc all-target-libgcc
	$(MAKE) -C $(GCC_BUILD) install-gcc install-target-libgcc

get-gcc:
	mkdir -p ./../../../toolchain
	cd ./../../../toolchain && wget $(GCC_URL) && tar -xvf gcc-$(GCC_VERSION).tar.xz