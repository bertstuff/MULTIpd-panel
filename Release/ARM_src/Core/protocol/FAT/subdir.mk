################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Core/protocol/FAT/diskio.c \
../ARM_src/Core/protocol/FAT/fattime.c \
../ARM_src/Core/protocol/FAT/ff.c \
../ARM_src/Core/protocol/FAT/ffsystem.c \
../ARM_src/Core/protocol/FAT/ffunicode.c \
../ARM_src/Core/protocol/FAT/sdcard.c 

OBJS += \
./ARM_src/Core/protocol/FAT/diskio.o \
./ARM_src/Core/protocol/FAT/fattime.o \
./ARM_src/Core/protocol/FAT/ff.o \
./ARM_src/Core/protocol/FAT/ffsystem.o \
./ARM_src/Core/protocol/FAT/ffunicode.o \
./ARM_src/Core/protocol/FAT/sdcard.o 

C_DEPS += \
./ARM_src/Core/protocol/FAT/diskio.d \
./ARM_src/Core/protocol/FAT/fattime.d \
./ARM_src/Core/protocol/FAT/ff.d \
./ARM_src/Core/protocol/FAT/ffsystem.d \
./ARM_src/Core/protocol/FAT/ffunicode.d \
./ARM_src/Core/protocol/FAT/sdcard.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Core/protocol/FAT/%.o: ../ARM_src/Core/protocol/FAT/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\Multipd\src" -I"D:\ARM\MULTIpd_software\Multipd\ARM_src" -Og -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


