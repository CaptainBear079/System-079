stage2_bootmanager:
	$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $(BUILD_DIR)/entry.o $(BOOTLOADER_STAGE2)/$(STAGE2_ENTRY)
	$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $(BUILD_DIR)/realmode.o $(BOOTLOADER_STAGE2)/$(STAGE2_REALMODE)
	$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $(BUILD_DIR)/protectedmode.o $(BOOTLOADER_STAGE2)/$(STAGE2_PROTECTEDMODE)
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(BUILD_DIR)/utils.o $(BOOTLOADER_STAGE2)/utils.c -D__32BIT__
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(BUILD_DIR)/restdio.o $(BOOTLOADER_STAGE2)/stdio.c -D__32BIT__
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(BUILD_DIR)/disk.o $(BOOTLOADER_STAGE2)/disk.c -D__32BIT__
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(BUILD_DIR)/bootmanager.o $(BOOTLOADER_STAGE2)/bootmanager.c
	$(TARGET_LINKER) $(TARGET_LINKER_FLAGS) -Wl,-Map=$(BUILD_DIR)/bootmanager.map -T $(BOOTLOADER_STAGE2)/linker.ld -o $(BUILD_DIR)/bootmanager.bin $(BUILD_DIR)/entry.o $(BUILD_DIR)/bootmanager.o $(BUILD_DIR)/restdio.o $(BUILD_DIR)/realmode.o $(BUILD_DIR)/protectedmode.o $(BUILD_DIR)/disk.o $(BUILD_DIR)/utils.o

clean-stage2-bootmanager:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/bootmanager.bin