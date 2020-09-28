################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Screen/subdir.mk
-include src/subdir.mk
-include ARM_src/Screen/Transaction/subdir.mk
-include ARM_src/Screen/Menu/Menu_screen/subdir.mk
-include ARM_src/Screen/Menu/Menu_handler/subdir.mk
-include ARM_src/Screen/Menu/subdir.mk
-include ARM_src/Screen/subdir.mk
-include ARM_src/Processes/subdir.mk
-include ARM_src/Driver/source/subdir.mk
-include ARM_src/Device/UART/XBA/subdir.mk
-include ARM_src/Device/UART/VK80/subdir.mk
-include ARM_src/Device/UART/TG2460H/subdir.mk
-include ARM_src/Device/UART/MDB-C21/subdir.mk
-include ARM_src/Device/UART/ELTRA/subdir.mk
-include ARM_src/Device/Net/payter/subdir.mk
-include ARM_src/Device/Net/CCV/subdir.mk
-include ARM_src/Device/IO/SERVICE/subdir.mk
-include ARM_src/Device/IO/IO/subdir.mk
-include ARM_src/Device/IO/BUTTONS/subdir.mk
-include ARM_src/Device/I2C/SC16IS740/subdir.mk
-include ARM_src/Device/I2C/RFID/subdir.mk
-include ARM_src/Device/I2C/MAX128/subdir.mk
-include ARM_src/Device/I2C/LV3095/subdir.mk
-include ARM_src/Device/I2C/LCD_Alphanumeric/subdir.mk
-include ARM_src/Device/I2C/IO_4Bit/subdir.mk
-include ARM_src/Device/I2C/Edip_240B/subdir.mk
-include ARM_src/Device/I2C/EMP_800/subdir.mk
-include ARM_src/Device/I2C/BUTTONS/subdir.mk
-include ARM_src/Device/I2C/9554/subdir.mk
-include ARM_src/Device/I2C/8574/subdir.mk
-include ARM_src/Device/subdir.mk
-include ARM_src/Core/protocol/Secure/sha1/subdir.mk
-include ARM_src/Core/protocol/Secure/aes/subdir.mk
-include ARM_src/Core/protocol/Net/ntp/subdir.mk
-include ARM_src/Core/protocol/Net/emac/subdir.mk
-include ARM_src/Core/protocol/Net/dns/subdir.mk
-include ARM_src/Core/protocol/Net/dhcp/subdir.mk
-include ARM_src/Core/protocol/Net/SCP/subdir.mk
-include ARM_src/Core/protocol/Net/Ejournal/subdir.mk
-include ARM_src/Core/protocol/Net/AMS/subdir.mk
-include ARM_src/Core/protocol/Net/subdir.mk
-include ARM_src/Core/protocol/FAT/subdir.mk
-include ARM_src/Core/protocol/Checksum/crc32/subdir.mk
-include ARM_src/Core/protocol/Checksum/crc16/subdir.mk
-include ARM_src/Core/Util/subdir.mk
-include ARM_src/Core/Type/subdir.mk
-include ARM_src/Core/Timer/subdir.mk
-include ARM_src/Core/Process/subdir.mk
-include ARM_src/Core/Peripherals/subdir.mk
-include ARM_src/Core/Memory/subdir.mk
-include ARM_src/Core/Math/subdir.mk
-include ARM_src/Core/Extensions/mxml/subdir.mk
-include ARM_src/Core/Extensions/ini/subdir.mk
-include ARM_src/Core/Extensions/code/subdir.mk
-include ARM_src/Core/Extensions/BMP_mono/subdir.mk
-include ARM_src/Core/Debug/subdir.mk
-include ARM_src/Core/Clock/subdir.mk
-include ARM_src/Board/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pre-build main-build

# Main-build Target
main-build: MULTIpd_panel.axf

dependents:
	-cd D:/ARM/Bootloader_software/CMSISv1p30_LPC17xx/Debug && $(MAKE) clean all

# Tool invocations
MULTIpd_panel.axf: $(OBJS) $(USER_OBJS) D:/ARM/Bootloader_software/CMSISv1p30_LPC17xx/Debug/libCMSISv1p30_LPC17xx.a
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Debug" -Xlinker -Map="MULTIpd_panel.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "MULTIpos_Debug.ld" -o "MULTIpd_panel.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) MULTIpd_panel.axf
	-@echo ' '

pre-build:
	-SubWCRev D:\ARM\MULTIpd_software\MULTIpd_panel  D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src\Core\Debug\Version.tmpl  D:\ARM\MULTIpd_software\MULTIpd_panel\src\version.c -E
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "MULTIpd_panel.axf" ; arm-none-eabi-objcopy -v -O binary "MULTIpd_panel.axf" "MULTIpd_panel.bin" ; checksum -p LPC1766 -d "MULTIpd_panel.bin" ; arm-none-eabi-objcopy -I binary "MULTIpd_panel.bin" -O ihex "MULTIpd_panel.hex"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build
D:/ARM/Bootloader_software/CMSISv1p30_LPC17xx/Debug/libCMSISv1p30_LPC17xx.a:

-include ../makefile.targets
