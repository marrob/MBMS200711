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
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../USB_DEVICE/Target -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/LED -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/Common/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/LED/Led.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Drivers/LED/LiveLed.o: ../Drivers/LED/LiveLed.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../USB_DEVICE/Target -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/LED -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/Common/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/LED/LiveLed.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

