################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LED/Led.c \
../Drivers/LED/LiveLed.c 

OBJS += \
./Drivers/LED/Led.o \
./Drivers/LED/LiveLed.o 

C_DEPS += \
./Drivers/LED/Led.d \
./Drivers/LED/LiveLed.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LED/Led.o: ../Drivers/LED/Led.c Drivers/LED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/LED -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/Common/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/LED/Led.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Drivers/LED/LiveLed.o: ../Drivers/LED/LiveLed.c Drivers/LED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/LED -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/Common/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/LED/LiveLed.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

