################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Core/Timer/ctimer.c \
../ARM_src/Core/Timer/etimer.c \
../ARM_src/Core/Timer/rtimer.c \
../ARM_src/Core/Timer/stimer.c \
../ARM_src/Core/Timer/timer.c \
../ARM_src/Core/Timer/timer_functions.c 

OBJS += \
./ARM_src/Core/Timer/ctimer.o \
./ARM_src/Core/Timer/etimer.o \
./ARM_src/Core/Timer/rtimer.o \
./ARM_src/Core/Timer/stimer.o \
./ARM_src/Core/Timer/timer.o \
./ARM_src/Core/Timer/timer_functions.o 

C_DEPS += \
./ARM_src/Core/Timer/ctimer.d \
./ARM_src/Core/Timer/etimer.d \
./ARM_src/Core/Timer/rtimer.d \
./ARM_src/Core/Timer/stimer.d \
./ARM_src/Core/Timer/timer.d \
./ARM_src/Core/Timer/timer_functions.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Core/Timer/%.o: ../ARM_src/Core/Timer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -DDEBUGGING -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


