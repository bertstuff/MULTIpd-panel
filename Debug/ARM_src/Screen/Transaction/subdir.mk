################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Screen/Transaction/SCR_Reservation.c \
../ARM_src/Screen/Transaction/SCR_Reservation_availability.c \
../ARM_src/Screen/Transaction/SCR_Transaction.c \
../ARM_src/Screen/Transaction/SCR_Transaction_ATM.c \
../ARM_src/Screen/Transaction/SCR_Transaction_Receipt.c \
../ARM_src/Screen/Transaction/SCR_Transaction_Reservation.c \
../ARM_src/Screen/Transaction/SCR_Transaction_init.c \
../ARM_src/Screen/Transaction/SCR_Transaction_products.c \
../ARM_src/Screen/Transaction/SCR_Transaction_statistics.c 

OBJS += \
./ARM_src/Screen/Transaction/SCR_Reservation.o \
./ARM_src/Screen/Transaction/SCR_Reservation_availability.o \
./ARM_src/Screen/Transaction/SCR_Transaction.o \
./ARM_src/Screen/Transaction/SCR_Transaction_ATM.o \
./ARM_src/Screen/Transaction/SCR_Transaction_Receipt.o \
./ARM_src/Screen/Transaction/SCR_Transaction_Reservation.o \
./ARM_src/Screen/Transaction/SCR_Transaction_init.o \
./ARM_src/Screen/Transaction/SCR_Transaction_products.o \
./ARM_src/Screen/Transaction/SCR_Transaction_statistics.o 

C_DEPS += \
./ARM_src/Screen/Transaction/SCR_Reservation.d \
./ARM_src/Screen/Transaction/SCR_Reservation_availability.d \
./ARM_src/Screen/Transaction/SCR_Transaction.d \
./ARM_src/Screen/Transaction/SCR_Transaction_ATM.d \
./ARM_src/Screen/Transaction/SCR_Transaction_Receipt.d \
./ARM_src/Screen/Transaction/SCR_Transaction_Reservation.d \
./ARM_src/Screen/Transaction/SCR_Transaction_init.d \
./ARM_src/Screen/Transaction/SCR_Transaction_products.d \
./ARM_src/Screen/Transaction/SCR_Transaction_statistics.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Screen/Transaction/%.o: ../ARM_src/Screen/Transaction/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -DDEBUGGING -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\Multipd\src" -I"D:\ARM\MULTIpd_software\Multipd\ARM_src" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


