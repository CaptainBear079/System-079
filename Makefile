
include ./build_scripts/config.mk
include ./dev/src/bootloader/stage1/stage1.mk
include ./dev/src/bootloader/stage2/stage2.mk
include ./dev/src/kernel/kernel.mk

.PHONY: all build-toolchain build-system gen-docs clean-build-toolchain

all:
	@echo "Specify target. make <target>"

target-stage1: stage1-bootloader-target

stage2: stage2-bootmanager-host

target-stage2: stage2-bootmanager-target

make-run-img: stage1-bootloader-target stage2-bootmanager-target build-kernel
	@echo "Running image for OS... Create image with the system..."
	dd if=/dev/zero of=$(BUILD_DIR)/os.img bs=512 count=13
	dd if=$(BUILD_DIR)/$(BOOTLOADER_STAGE1_BIN) of=$(BUILD_DIR)/os.img conv=notrunc bs=512 count=1
	dd if=$(BUILD_DIR)/$(BOOTLOADER_STAGE2_BIN) of=$(BUILD_DIR)/os.img conv=notrunc bs=512 seek=512
	dd if=$(BUILD_DIR)/$(KERNEL_BIN) of=$(BUILD_DIR)/os.img conv=sync bs=512 seek=6684
	@echo $(EMULATOR) $(EMULATOR_FLAGS)

gen-docs:

clean-build-toolchain:
	rm -rf toolchain/*

clean-toolchain-downloads:
	rm -rf toolchain/*.tar.xz

clean-build-system:
	rm -rf $(BUILD_DIR)/*.o
