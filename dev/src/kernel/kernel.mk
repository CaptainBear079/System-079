build-kernel:
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(BUILD_DIR)/ckernel.o $(KERNEL)/kernel.c
	$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $(BUILD_DIR)/kentry.o $(KERNEL)/entry.asm
	$(TARGET_LINKER) $(TARGET_LINKER_FLAGS) -Wl,-Map=$(BUILD_DIR)/kernel.map -T $(KERNEL)/linker.ld -o $(BUILD_DIR)/kernel.bin $(BUILD_DIR)/ckernel.o $(BUILD_DIR)/entry.o