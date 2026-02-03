include ./../../build_scripts/config.mk
include ./bootloader/stage1/stage1.mk
include ./bootloader/stage2/stage2.mk
include ./kernel/kernel.mk

create_build_dir:
	mkdir -p $(BUILD_DIR)

build_bootloader: stage1_bootloader stage2_bootmanager
	cat $(BUILD_DIR)/bootloader_stage1.bin $(BUILD_DIR)/bootloader_stage2.bin > $(BUILD_DIR)/bootloader.bin

build_kernel_i686: kernel_i686 save_os_image

build_kernel_x86_64: kernel_x86_64 save_os_image

save_os_image:build_bootloader
	cat $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/os.img
