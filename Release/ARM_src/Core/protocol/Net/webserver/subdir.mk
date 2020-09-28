################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Core/protocol/Net/webserver/http-strings.c \
../ARM_src/Core/protocol/Net/webserver/httpd-cfs.c \
../ARM_src/Core/protocol/Net/webserver/httpd-cgi.c \
../ARM_src/Core/protocol/Net/webserver/httpd-fs.c \
../ARM_src/Core/protocol/Net/webserver/httpd-fsdata.c \
../ARM_src/Core/protocol/Net/webserver/httpd.c \
../ARM_src/Core/protocol/Net/webserver/urlconv.c \
../ARM_src/Core/protocol/Net/webserver/webserver.c 

OBJS += \
./ARM_src/Core/protocol/Net/webserver/http-strings.o \
./ARM_src/Core/protocol/Net/webserver/httpd-cfs.o \
./ARM_src/Core/protocol/Net/webserver/httpd-cgi.o \
./ARM_src/Core/protocol/Net/webserver/httpd-fs.o \
./ARM_src/Core/protocol/Net/webserver/httpd-fsdata.o \
./ARM_src/Core/protocol/Net/webserver/httpd.o \
./ARM_src/Core/protocol/Net/webserver/urlconv.o \
./ARM_src/Core/protocol/Net/webserver/webserver.o 

C_DEPS += \
./ARM_src/Core/protocol/Net/webserver/http-strings.d \
./ARM_src/Core/protocol/Net/webserver/httpd-cfs.d \
./ARM_src/Core/protocol/Net/webserver/httpd-cgi.d \
./ARM_src/Core/protocol/Net/webserver/httpd-fs.d \
./ARM_src/Core/protocol/Net/webserver/httpd-fsdata.d \
./ARM_src/Core/protocol/Net/webserver/httpd.d \
./ARM_src/Core/protocol/Net/webserver/urlconv.d \
./ARM_src/Core/protocol/Net/webserver/webserver.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Core/protocol/Net/webserver/%.o: ../ARM_src/Core/protocol/Net/webserver/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


