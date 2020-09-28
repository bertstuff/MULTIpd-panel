################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Core/Extensions/mxml/mxml-attr.c \
../ARM_src/Core/Extensions/mxml/mxml-entity.c \
../ARM_src/Core/Extensions/mxml/mxml-file.c \
../ARM_src/Core/Extensions/mxml/mxml-get.c \
../ARM_src/Core/Extensions/mxml/mxml-index.c \
../ARM_src/Core/Extensions/mxml/mxml-memory.c \
../ARM_src/Core/Extensions/mxml/mxml-node.c \
../ARM_src/Core/Extensions/mxml/mxml-private.c \
../ARM_src/Core/Extensions/mxml/mxml-search.c \
../ARM_src/Core/Extensions/mxml/mxml-set.c \
../ARM_src/Core/Extensions/mxml/mxml-string.c 

OBJS += \
./ARM_src/Core/Extensions/mxml/mxml-attr.o \
./ARM_src/Core/Extensions/mxml/mxml-entity.o \
./ARM_src/Core/Extensions/mxml/mxml-file.o \
./ARM_src/Core/Extensions/mxml/mxml-get.o \
./ARM_src/Core/Extensions/mxml/mxml-index.o \
./ARM_src/Core/Extensions/mxml/mxml-memory.o \
./ARM_src/Core/Extensions/mxml/mxml-node.o \
./ARM_src/Core/Extensions/mxml/mxml-private.o \
./ARM_src/Core/Extensions/mxml/mxml-search.o \
./ARM_src/Core/Extensions/mxml/mxml-set.o \
./ARM_src/Core/Extensions/mxml/mxml-string.o 

C_DEPS += \
./ARM_src/Core/Extensions/mxml/mxml-attr.d \
./ARM_src/Core/Extensions/mxml/mxml-entity.d \
./ARM_src/Core/Extensions/mxml/mxml-file.d \
./ARM_src/Core/Extensions/mxml/mxml-get.d \
./ARM_src/Core/Extensions/mxml/mxml-index.d \
./ARM_src/Core/Extensions/mxml/mxml-memory.d \
./ARM_src/Core/Extensions/mxml/mxml-node.d \
./ARM_src/Core/Extensions/mxml/mxml-private.d \
./ARM_src/Core/Extensions/mxml/mxml-search.d \
./ARM_src/Core/Extensions/mxml/mxml-set.d \
./ARM_src/Core/Extensions/mxml/mxml-string.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Core/Extensions/mxml/%.o: ../ARM_src/Core/Extensions/mxml/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


