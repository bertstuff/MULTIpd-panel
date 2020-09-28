################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Screen/Menu/Menu_screen/SCR_ATM_menu.c \
../ARM_src/Screen/Menu/Menu_screen/SCR_Bill_menu.c \
../ARM_src/Screen/Menu/Menu_screen/SCR_Cash_menu.c \
../ARM_src/Screen/Menu/Menu_screen/SCR_Info_menu.c \
../ARM_src/Screen/Menu/Menu_screen/SCR_NET_menu.c \
../ARM_src/Screen/Menu/Menu_screen/SCR_Pass_menu.c \
../ARM_src/Screen/Menu/Menu_screen/SCR_Revenue_menu.c 

OBJS += \
./ARM_src/Screen/Menu/Menu_screen/SCR_ATM_menu.o \
./ARM_src/Screen/Menu/Menu_screen/SCR_Bill_menu.o \
./ARM_src/Screen/Menu/Menu_screen/SCR_Cash_menu.o \
./ARM_src/Screen/Menu/Menu_screen/SCR_Info_menu.o \
./ARM_src/Screen/Menu/Menu_screen/SCR_NET_menu.o \
./ARM_src/Screen/Menu/Menu_screen/SCR_Pass_menu.o \
./ARM_src/Screen/Menu/Menu_screen/SCR_Revenue_menu.o 

C_DEPS += \
./ARM_src/Screen/Menu/Menu_screen/SCR_ATM_menu.d \
./ARM_src/Screen/Menu/Menu_screen/SCR_Bill_menu.d \
./ARM_src/Screen/Menu/Menu_screen/SCR_Cash_menu.d \
./ARM_src/Screen/Menu/Menu_screen/SCR_Info_menu.d \
./ARM_src/Screen/Menu/Menu_screen/SCR_NET_menu.d \
./ARM_src/Screen/Menu/Menu_screen/SCR_Pass_menu.d \
./ARM_src/Screen/Menu/Menu_screen/SCR_Revenue_menu.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Screen/Menu/Menu_screen/%.o: ../ARM_src/Screen/Menu/Menu_screen/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


