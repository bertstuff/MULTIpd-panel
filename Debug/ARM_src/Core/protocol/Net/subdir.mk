################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Core/protocol/Net/psock.c \
../ARM_src/Core/protocol/Net/tcpip.c \
../ARM_src/Core/protocol/Net/uip-debug.c \
../ARM_src/Core/protocol/Net/uip-packetqueue.c \
../ARM_src/Core/protocol/Net/uip-split.c \
../ARM_src/Core/protocol/Net/uip-udp-packet.c \
../ARM_src/Core/protocol/Net/uip.c \
../ARM_src/Core/protocol/Net/uip_arp.c \
../ARM_src/Core/protocol/Net/uiplib.c 

OBJS += \
./ARM_src/Core/protocol/Net/psock.o \
./ARM_src/Core/protocol/Net/tcpip.o \
./ARM_src/Core/protocol/Net/uip-debug.o \
./ARM_src/Core/protocol/Net/uip-packetqueue.o \
./ARM_src/Core/protocol/Net/uip-split.o \
./ARM_src/Core/protocol/Net/uip-udp-packet.o \
./ARM_src/Core/protocol/Net/uip.o \
./ARM_src/Core/protocol/Net/uip_arp.o \
./ARM_src/Core/protocol/Net/uiplib.o 

C_DEPS += \
./ARM_src/Core/protocol/Net/psock.d \
./ARM_src/Core/protocol/Net/tcpip.d \
./ARM_src/Core/protocol/Net/uip-debug.d \
./ARM_src/Core/protocol/Net/uip-packetqueue.d \
./ARM_src/Core/protocol/Net/uip-split.d \
./ARM_src/Core/protocol/Net/uip-udp-packet.d \
./ARM_src/Core/protocol/Net/uip.d \
./ARM_src/Core/protocol/Net/uip_arp.d \
./ARM_src/Core/protocol/Net/uiplib.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Core/protocol/Net/%.o: ../ARM_src/Core/protocol/Net/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -DDEBUGGING -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\Multipd\src" -I"D:\ARM\MULTIpd_software\Multipd\ARM_src" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


