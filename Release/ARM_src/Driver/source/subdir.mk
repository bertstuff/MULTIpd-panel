################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ARM_src/Driver/source/debug_frmwrk.c \
../ARM_src/Driver/source/lpc17xx_adc.c \
../ARM_src/Driver/source/lpc17xx_can.c \
../ARM_src/Driver/source/lpc17xx_clkpwr.c \
../ARM_src/Driver/source/lpc17xx_dac.c \
../ARM_src/Driver/source/lpc17xx_emac.c \
../ARM_src/Driver/source/lpc17xx_exti.c \
../ARM_src/Driver/source/lpc17xx_gpdma.c \
../ARM_src/Driver/source/lpc17xx_gpio.c \
../ARM_src/Driver/source/lpc17xx_i2c.c \
../ARM_src/Driver/source/lpc17xx_i2s.c \
../ARM_src/Driver/source/lpc17xx_libcfg_default.c \
../ARM_src/Driver/source/lpc17xx_mcpwm.c \
../ARM_src/Driver/source/lpc17xx_nvic.c \
../ARM_src/Driver/source/lpc17xx_pinsel.c \
../ARM_src/Driver/source/lpc17xx_pwm.c \
../ARM_src/Driver/source/lpc17xx_qei.c \
../ARM_src/Driver/source/lpc17xx_rit.c \
../ARM_src/Driver/source/lpc17xx_rtc.c \
../ARM_src/Driver/source/lpc17xx_spi.c \
../ARM_src/Driver/source/lpc17xx_ssp.c \
../ARM_src/Driver/source/lpc17xx_systick.c \
../ARM_src/Driver/source/lpc17xx_timer.c \
../ARM_src/Driver/source/lpc17xx_uart.c \
../ARM_src/Driver/source/lpc17xx_wdt.c 

OBJS += \
./ARM_src/Driver/source/debug_frmwrk.o \
./ARM_src/Driver/source/lpc17xx_adc.o \
./ARM_src/Driver/source/lpc17xx_can.o \
./ARM_src/Driver/source/lpc17xx_clkpwr.o \
./ARM_src/Driver/source/lpc17xx_dac.o \
./ARM_src/Driver/source/lpc17xx_emac.o \
./ARM_src/Driver/source/lpc17xx_exti.o \
./ARM_src/Driver/source/lpc17xx_gpdma.o \
./ARM_src/Driver/source/lpc17xx_gpio.o \
./ARM_src/Driver/source/lpc17xx_i2c.o \
./ARM_src/Driver/source/lpc17xx_i2s.o \
./ARM_src/Driver/source/lpc17xx_libcfg_default.o \
./ARM_src/Driver/source/lpc17xx_mcpwm.o \
./ARM_src/Driver/source/lpc17xx_nvic.o \
./ARM_src/Driver/source/lpc17xx_pinsel.o \
./ARM_src/Driver/source/lpc17xx_pwm.o \
./ARM_src/Driver/source/lpc17xx_qei.o \
./ARM_src/Driver/source/lpc17xx_rit.o \
./ARM_src/Driver/source/lpc17xx_rtc.o \
./ARM_src/Driver/source/lpc17xx_spi.o \
./ARM_src/Driver/source/lpc17xx_ssp.o \
./ARM_src/Driver/source/lpc17xx_systick.o \
./ARM_src/Driver/source/lpc17xx_timer.o \
./ARM_src/Driver/source/lpc17xx_uart.o \
./ARM_src/Driver/source/lpc17xx_wdt.o 

C_DEPS += \
./ARM_src/Driver/source/debug_frmwrk.d \
./ARM_src/Driver/source/lpc17xx_adc.d \
./ARM_src/Driver/source/lpc17xx_can.d \
./ARM_src/Driver/source/lpc17xx_clkpwr.d \
./ARM_src/Driver/source/lpc17xx_dac.d \
./ARM_src/Driver/source/lpc17xx_emac.d \
./ARM_src/Driver/source/lpc17xx_exti.d \
./ARM_src/Driver/source/lpc17xx_gpdma.d \
./ARM_src/Driver/source/lpc17xx_gpio.d \
./ARM_src/Driver/source/lpc17xx_i2c.d \
./ARM_src/Driver/source/lpc17xx_i2s.d \
./ARM_src/Driver/source/lpc17xx_libcfg_default.d \
./ARM_src/Driver/source/lpc17xx_mcpwm.d \
./ARM_src/Driver/source/lpc17xx_nvic.d \
./ARM_src/Driver/source/lpc17xx_pinsel.d \
./ARM_src/Driver/source/lpc17xx_pwm.d \
./ARM_src/Driver/source/lpc17xx_qei.d \
./ARM_src/Driver/source/lpc17xx_rit.d \
./ARM_src/Driver/source/lpc17xx_rtc.d \
./ARM_src/Driver/source/lpc17xx_spi.d \
./ARM_src/Driver/source/lpc17xx_ssp.d \
./ARM_src/Driver/source/lpc17xx_systick.d \
./ARM_src/Driver/source/lpc17xx_timer.d \
./ARM_src/Driver/source/lpc17xx_uart.d \
./ARM_src/Driver/source/lpc17xx_wdt.d 


# Each subdirectory must supply rules for building sources it contributes
ARM_src/Driver/source/%.o: ../ARM_src/Driver/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__LPC17XX__ -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\CMSIS\Include" -I"D:\ARM\Bootloader_software\CMSISv1p30_LPC17xx\Device\NXP\LPC17xx\Include" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\src" -I"D:\ARM\MULTIpd_software\MULTIpd_panel\ARM_src" -Og -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


