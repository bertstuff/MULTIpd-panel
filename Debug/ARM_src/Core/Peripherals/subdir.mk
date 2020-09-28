################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Core/Peripherals/I2C_Dev_Init.c \
../ARM_src/Core/Peripherals/Net_Dev_Init.c \
../ARM_src/Core/Peripherals/UART_Dev_Init.c 

OBJS += \
./ARM_src/Core/Peripherals/I2C_Dev_Init.o \
./ARM_src/Core/Peripherals/Net_Dev_Init.o \
./ARM_src/Core/Peripherals/UART_Dev_Init.o 

C_DEPS += \
./ARM_src/Core/Peripherals/I2C_Dev_Init.d \
./ARM_src/Core/Peripherals/Net_Dev_Init.d \
./ARM_src/Core/Peripherals/UART_Dev_Init.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Core/Peripherals/%.o: ../ARM_src/Core/Peripherals/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -DDEBUGGING -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


