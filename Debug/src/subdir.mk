################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Energy_point.c \
../src/SCP_handler.c \
../src/Terminal_functions.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/main.c \
../src/version.c 

OBJS += \
./src/Energy_point.o \
./src/SCP_handler.o \
./src/Terminal_functions.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/main.o \
./src/version.o 

C_DEPS += \
./src/Energy_point.d \
./src/SCP_handler.d \
./src/Terminal_functions.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/main.d \
./src/version.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -DDEBUGGING -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


