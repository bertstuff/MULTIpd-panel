################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Core/Clock/Time_functions.c \
../ARM_src/Core/Clock/clock.c \
../ARM_src/Core/Clock/rtc_cal.c 

OBJS += \
./ARM_src/Core/Clock/Time_functions.o \
./ARM_src/Core/Clock/clock.o \
./ARM_src/Core/Clock/rtc_cal.o 

C_DEPS += \
./ARM_src/Core/Clock/Time_functions.d \
./ARM_src/Core/Clock/clock.d \
./ARM_src/Core/Clock/rtc_cal.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Core/Clock/%.o: ../ARM_src/Core/Clock/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\Multipd\src" -I"D:\ARM\MULTIpd_software\Multipd\ARM_src" -Og -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


