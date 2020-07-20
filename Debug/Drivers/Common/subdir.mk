################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Common/StringPlus.c \
../Drivers/Common/common.c 

OBJS += \
./Drivers/Common/StringPlus.o \
./Drivers/Common/common.o 

C_DEPS += \
./Drivers/Common/StringPlus.d \
./Drivers/Common/common.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Common/StringPlus.o: ../Drivers/Common/StringPlus.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../USB_DEVICE/Target -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/LED -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/Common -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Common/StringPlus.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Drivers/Common/common.o: ../Drivers/Common/common.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../USB_DEVICE/Target -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/LED -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/Common -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Common/common.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

