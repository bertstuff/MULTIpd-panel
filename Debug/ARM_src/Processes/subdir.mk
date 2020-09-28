################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Processes/Global_UID.c \
../ARM_src/Processes/IO_General_handlers.c \
../ARM_src/Processes/IO_process.c \
../ARM_src/Processes/Reservation.c \
../ARM_src/Processes/Servicebutton.c \
../ARM_src/Processes/dev_reset.c 

OBJS += \
./ARM_src/Processes/Global_UID.o \
./ARM_src/Processes/IO_General_handlers.o \
./ARM_src/Processes/IO_process.o \
./ARM_src/Processes/Reservation.o \
./ARM_src/Processes/Servicebutton.o \
./ARM_src/Processes/dev_reset.o 

C_DEPS += \
./ARM_src/Processes/Global_UID.d \
./ARM_src/Processes/IO_General_handlers.d \
./ARM_src/Processes/IO_process.d \
./ARM_src/Processes/Reservation.d \
./ARM_src/Processes/Servicebutton.d \
./ARM_src/Processes/dev_reset.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Processes/%.o: ../ARM_src/Processes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -DDEBUGGING -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


