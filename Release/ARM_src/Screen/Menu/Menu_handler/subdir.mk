################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_auth.c \
../ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_enum.c \
../ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_int.c \
../ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_menu.c \
../ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_message.c \
../ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_question.c \
../ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_text.c \
../ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_time.c 

OBJS += \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_auth.o \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_enum.o \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_int.o \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_menu.o \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_message.o \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_question.o \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_text.o \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_time.o 

C_DEPS += \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_auth.d \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_enum.d \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_int.d \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_menu.d \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_message.d \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_question.d \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_text.d \
./ARM_src/Screen/Menu/Menu_handler/SCR_Menu_handler_time.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Screen/Menu/Menu_handler/%.o: ../ARM_src/Screen/Menu/Menu_handler/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


