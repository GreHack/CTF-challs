################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/aes.c \
../Src/main.c \
../Src/sha-256.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/system_stm32f1xx.c 

OBJS += \
./Src/aes.o \
./Src/main.o \
./Src/sha-256.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/system_stm32f1xx.o 

C_DEPS += \
./Src/aes.d \
./Src/main.d \
./Src/sha-256.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -std=c99 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"D:/STM32/Datas/STM32CubeMX/GH18-BluePill-01/Inc" -I"D:/STM32/Datas/STM32CubeMX/GH18-BluePill-01/Drivers/STM32F1xx_HAL_Driver/Inc" -I"D:/STM32/Datas/STM32CubeMX/GH18-BluePill-01/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"D:/STM32/Datas/STM32CubeMX/GH18-BluePill-01/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"D:/STM32/Datas/STM32CubeMX/GH18-BluePill-01/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


