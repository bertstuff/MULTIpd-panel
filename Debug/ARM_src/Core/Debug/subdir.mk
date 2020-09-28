################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Core/Debug/Debug_Terminal.c \
../ARM_src/Core/Debug/Error_check.c \
../ARM_src/Core/Debug/Hard_Fault_handler.c \
../ARM_src/Core/Debug/Print_device_info.c \
../ARM_src/Core/Debug/Serial_Terminal.c \
../ARM_src/Core/Debug/TCP_Terminal.c 

OBJS += \
./ARM_src/Core/Debug/Debug_Terminal.o \
./ARM_src/Core/Debug/Error_check.o \
./ARM_src/Core/Debug/Hard_Fault_handler.o \
./ARM_src/Core/Debug/Print_device_info.o \
./ARM_src/Core/Debug/Serial_Terminal.o \
./ARM_src/Core/Debug/TCP_Terminal.o 

C_DEPS += \
./ARM_src/Core/Debug/Debug_Terminal.d \
./ARM_src/Core/Debug/Error_check.d \
./ARM_src/Core/Debug/Hard_Fault_handler.d \
./ARM_src/Core/Debug/Print_device_info.d \
./ARM_src/Core/Debug/Serial_Terminal.d \
./ARM_src/Core/Debug/TCP_Terminal.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Core/Debug/%.o: ../ARM_src/Core/Debug/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -DDEBUGGING -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


