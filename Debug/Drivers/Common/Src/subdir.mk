################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Common/Src/StringPlus.c \
../Drivers/Common/Src/common.c 

OBJS += \
./Drivers/Common/Src/StringPlus.o \
./Drivers/Common/Src/common.o 

C_DEPS += \
./Drivers/Common/Src/StringPlus.d \
./Drivers/Common/Src/common.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Common/Src/StringPlus.o: ../Drivers/Common/Src/StringPlus.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../USB_DEVICE/Target -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/LED -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/Common/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Common/Src/StringPlus.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
Drivers/Common/Src/common.o: ../Drivers/Common/Src/common.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../USB_DEVICE/Target -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/LED -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/Common/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Common/Src/common.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

