################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Screen/SCR_Idle.c \
../src/Screen/SCR_Language.c \
../src/Screen/SCR_Overview.c \
../src/Screen/SCR_Text.c \
../src/Screen/SCR_pin.c 

OBJS += \
./src/Screen/SCR_Idle.o \
./src/Screen/SCR_Language.o \
./src/Screen/SCR_Overview.o \
./src/Screen/SCR_Text.o \
./src/Screen/SCR_pin.o 

C_DEPS += \
./src/Screen/SCR_Idle.d \
./src/Screen/SCR_Language.d \
./src/Screen/SCR_Overview.d \
./src/Screen/SCR_Text.d \
./src/Screen/SCR_pin.d 


# Each subdirectory must supply rules for building sources it contributes
src/Screen/%.o: ../src/Screen/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -DDEBUGGING -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


