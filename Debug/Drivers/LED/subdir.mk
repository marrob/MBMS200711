################################################################################
# Automatically-generated file. Do not edit!
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
Drivers/LED/Led.o: ../Drivers/LED/Led.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/LED -I../Drivers/Common -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/LED/Led.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Drivers/LED/LiveLed.o: ../Drivers/LED/LiveLed.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/LED -I../Drivers/Common -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/LED/LiveLed.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

