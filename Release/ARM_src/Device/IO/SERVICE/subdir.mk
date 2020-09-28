################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Device/IO/SERVICE/Debug_led.c \
../ARM_src/Device/IO/SERVICE/Service_IO.c \
../ARM_src/Device/IO/SERVICE/Servicepin.c 

OBJS += \
./ARM_src/Device/IO/SERVICE/Debug_led.o \
./ARM_src/Device/IO/SERVICE/Service_IO.o \
./ARM_src/Device/IO/SERVICE/Servicepin.o 

C_DEPS += \
./ARM_src/Device/IO/SERVICE/Debug_led.d \
./ARM_src/Device/IO/SERVICE/Service_IO.d \
./ARM_src/Device/IO/SERVICE/Servicepin.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Device/IO/SERVICE/%.o: ../ARM_src/Device/IO/SERVICE/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


