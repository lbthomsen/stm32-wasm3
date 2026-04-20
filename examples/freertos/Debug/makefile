################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include ws2812/subdir.mk
-include wasm3/extensions/subdir.mk
-include wasm3/subdir.mk
-include tinyusb/typec/subdir.mk
-include tinyusb/portable/wch/subdir.mk
-include tinyusb/portable/valentyusb/eptri/subdir.mk
-include tinyusb/portable/ti/msp430x5xx/subdir.mk
-include tinyusb/portable/template/subdir.mk
-include tinyusb/portable/synopsys/dwc2/subdir.mk
-include tinyusb/portable/sunxi/subdir.mk
-include tinyusb/portable/st/typec/subdir.mk
-include tinyusb/portable/st/stm32_fsdev/subdir.mk
-include tinyusb/portable/sony/cxd56/subdir.mk
-include tinyusb/portable/renesas/rusb2/subdir.mk
-include tinyusb/portable/raspberrypi/rp2040/subdir.mk
-include tinyusb/portable/raspberrypi/pio_usb/subdir.mk
-include tinyusb/portable/ohci/subdir.mk
-include tinyusb/portable/nxp/lpc_ip3511/subdir.mk
-include tinyusb/portable/nxp/lpc17_40/subdir.mk
-include tinyusb/portable/nxp/khci/subdir.mk
-include tinyusb/portable/nuvoton/nuc505/subdir.mk
-include tinyusb/portable/nuvoton/nuc121/subdir.mk
-include tinyusb/portable/nuvoton/nuc120/subdir.mk
-include tinyusb/portable/nordic/nrf5x/subdir.mk
-include tinyusb/portable/mindmotion/mm32/subdir.mk
-include tinyusb/portable/microchip/samx7x/subdir.mk
-include tinyusb/portable/microchip/samg/subdir.mk
-include tinyusb/portable/microchip/samd/subdir.mk
-include tinyusb/portable/microchip/pic32mz/subdir.mk
-include tinyusb/portable/microchip/pic/subdir.mk
-include tinyusb/portable/mentor/musb/subdir.mk
-include tinyusb/portable/ehci/subdir.mk
-include tinyusb/portable/dialog/da146xx/subdir.mk
-include tinyusb/portable/chipidea/ci_hs/subdir.mk
-include tinyusb/portable/chipidea/ci_fs/subdir.mk
-include tinyusb/portable/bridgetek/ft9xx/subdir.mk
-include tinyusb/portable/analog/max3421/subdir.mk
-include tinyusb/host/subdir.mk
-include tinyusb/device/subdir.mk
-include tinyusb/common/subdir.mk
-include tinyusb/class/video/subdir.mk
-include tinyusb/class/vendor/subdir.mk
-include tinyusb/class/usbtmc/subdir.mk
-include tinyusb/class/net/subdir.mk
-include tinyusb/class/mtp/subdir.mk
-include tinyusb/class/msc/subdir.mk
-include tinyusb/class/midi/subdir.mk
-include tinyusb/class/hid/subdir.mk
-include tinyusb/class/dfu/subdir.mk
-include tinyusb/class/cdc/subdir.mk
-include tinyusb/class/bth/subdir.mk
-include tinyusb/class/audio/subdir.mk
-include tinyusb/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := freertos
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
freertos.elf \

MAP_FILES += \
freertos.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
freertos.list \


# All Target
all: main-build

# Main-build Target
main-build: freertos.elf secondary-outputs

# Tool invocations
freertos.elf freertos.map: $(OBJS) $(USER_OBJS) /home/lth/src/stm32-wasm3/examples/freertos/STM32F407VETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "freertos.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/lth/src/stm32-wasm3/examples/freertos/STM32F407VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="freertos.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

freertos.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "freertos.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout freertos.elf freertos.list freertos.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
