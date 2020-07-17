################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Drivers/LED/subdir.mk
-include Drivers/Common/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
MBMS200711.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
MBMS200711.list \

OBJCOPY_BIN += \
MBMS200711.bin \


# All Target
all: MBMS200711.elf secondary-outputs

# Tool invocations
MBMS200711.elf: $(OBJS) $(USER_OBJS) D:\@@@!ProjectS\MFWL130221\STM32CubeIDE\MBMS200711\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "MBMS200711.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\@@@!ProjectS\MFWL130221\STM32CubeIDE\MBMS200711\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="MBMS200711.map" -Wl,--gc-sections -static  -mfloat-abi=soft -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

MBMS200711.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "MBMS200711.list"
	@echo 'Finished building: $@'
	@echo ' '

MBMS200711.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "MBMS200711.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
