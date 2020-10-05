################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Device/I2C/Edip_240B/EDIP_240B_Bargraph.c \
../ARM_src/Device/I2C/Edip_240B/EDIP_240B_Clipboard.c \
../ARM_src/Device/I2C/Edip_240B/EDIP_240B_Display.c \
../ARM_src/Device/I2C/Edip_240B/EDIP_240B_Draw.c \
../ARM_src/Device/I2C/Edip_240B/EDIP_240B_IO.c \
../ARM_src/Device/I2C/Edip_240B/EDIP_240B_Image.c \
../ARM_src/Device/I2C/Edip_240B/EDIP_240B_Init.c \
../ARM_src/Device/I2C/Edip_240B/EDIP_240B_Send.c \
../ARM_src/Device/I2C/Edip_240B/EDIP_240B_Text.c \
../ARM_src/Device/I2C/Edip_240B/EDIP_240B_Touch.c 

OBJS += \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Bargraph.o \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Clipboard.o \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Display.o \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Draw.o \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_IO.o \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Image.o \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Init.o \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Send.o \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Text.o \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Touch.o 

C_DEPS += \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Bargraph.d \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Clipboard.d \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Display.d \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Draw.d \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_IO.d \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Image.d \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Init.d \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Send.d \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Text.d \
./ARM_src/Device/I2C/Edip_240B/EDIP_240B_Touch.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Device/I2C/Edip_240B/%.o: ../ARM_src/Device/I2C/Edip_240B/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -DDEBUGGING -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\Multipd\src" -I"D:\ARM\MULTIpd_software\Multipd\ARM_src" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


