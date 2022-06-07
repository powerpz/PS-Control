################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AD5676.c \
../Core/Src/Def_Control.c \
../Core/Src/Def_ProtocolSlave.c \
../Core/Src/Def_RS485.c \
../Core/Src/Def_Standby.c \
../Core/Src/Def_System.c \
../Core/Src/Delays.c \
../Core/Src/LED.c \
../Core/Src/Ticks.c \
../Core/Src/UART.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/AD5676.o \
./Core/Src/Def_Control.o \
./Core/Src/Def_ProtocolSlave.o \
./Core/Src/Def_RS485.o \
./Core/Src/Def_Standby.o \
./Core/Src/Def_System.o \
./Core/Src/Delays.o \
./Core/Src/LED.o \
./Core/Src/Ticks.o \
./Core/Src/UART.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/AD5676.d \
./Core/Src/Def_Control.d \
./Core/Src/Def_ProtocolSlave.d \
./Core/Src/Def_RS485.d \
./Core/Src/Def_Standby.d \
./Core/Src/Def_System.d \
./Core/Src/Delays.d \
./Core/Src/LED.d \
./Core/Src/Ticks.d \
./Core/Src/UART.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AD5676.d ./Core/Src/AD5676.o ./Core/Src/AD5676.su ./Core/Src/Def_Control.d ./Core/Src/Def_Control.o ./Core/Src/Def_Control.su ./Core/Src/Def_ProtocolSlave.d ./Core/Src/Def_ProtocolSlave.o ./Core/Src/Def_ProtocolSlave.su ./Core/Src/Def_RS485.d ./Core/Src/Def_RS485.o ./Core/Src/Def_RS485.su ./Core/Src/Def_Standby.d ./Core/Src/Def_Standby.o ./Core/Src/Def_Standby.su ./Core/Src/Def_System.d ./Core/Src/Def_System.o ./Core/Src/Def_System.su ./Core/Src/Delays.d ./Core/Src/Delays.o ./Core/Src/Delays.su ./Core/Src/LED.d ./Core/Src/LED.o ./Core/Src/LED.su ./Core/Src/Ticks.d ./Core/Src/Ticks.o ./Core/Src/Ticks.su ./Core/Src/UART.d ./Core/Src/UART.o ./Core/Src/UART.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

