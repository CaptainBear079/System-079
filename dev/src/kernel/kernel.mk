build-kernel:
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(BUILD_DIR)/kernel_stdio.o $(KERNEL)/language_support/C/Standard_Libraries/stdio.c -D__32BIT__
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(BUILD_DIR)/kernel_other.o $(KERNEL)/other.c -D__32BIT__
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(BUILD_DIR)/ckernel.o $(KERNEL)/kernel.c -D__32BIT__
	$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $(BUILD_DIR)/kernel_asm.o $(KERNEL)/x86_assembly/protectedmode.asm
	$(TARGET_LINKER) $(TARGET_LINKER_FLAGS) -Wl,-Map=$(BUILD_DIR)/kernel.map -T $(KERNEL)/linker.ld -o $(BUILD_DIR)/kernel.bin $(BUILD_DIR)/ckernel.o $(BUILD_DIR)/kernel_asm.o $(BUILD_DIR)/kernel_stdio.o $(BUILD_DIR)/kernel_other.o