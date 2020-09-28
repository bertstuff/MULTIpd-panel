################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Core/Util/U_string.c \
../ARM_src/Core/Util/array.c \
../ARM_src/Core/Util/build_defs.c \
../ARM_src/Core/Util/math_util.c \
../ARM_src/Core/Util/petsciiconv.c 

OBJS += \
./ARM_src/Core/Util/U_string.o \
./ARM_src/Core/Util/array.o \
./ARM_src/Core/Util/build_defs.o \
./ARM_src/Core/Util/math_util.o \
./ARM_src/Core/Util/petsciiconv.o 

C_DEPS += \
./ARM_src/Core/Util/U_string.d \
./ARM_src/Core/Util/array.d \
./ARM_src/Core/Util/build_defs.d \
./ARM_src/Core/Util/math_util.d \
./ARM_src/Core/Util/petsciiconv.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Core/Util/%.o: ../ARM_src/Core/Util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


