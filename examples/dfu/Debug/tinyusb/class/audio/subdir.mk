################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/class/audio/audio_device.c 

OBJS += \
./tinyusb/class/audio/audio_device.o 

C_DEPS += \
./tinyusb/class/audio/audio_device.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/class/audio/%.o tinyusb/class/audio/%.su tinyusb/class/audio/%.cyclo: ../tinyusb/class/audio/%.c tinyusb/class/audio/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lth/src/stm32-wasm3/examples/dfu/wasm3" -I"/home/lth/src/stm32-wasm3/examples/dfu/ws2812" -I"/home/lth/src/stm32-wasm3/examples/dfu/tinyusb" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-class-2f-audio

clean-tinyusb-2f-class-2f-audio:
	-$(RM) ./tinyusb/class/audio/audio_device.cyclo ./tinyusb/class/audio/audio_device.d ./tinyusb/class/audio/audio_device.o ./tinyusb/class/audio/audio_device.su

.PHONY: clean-tinyusb-2f-class-2f-audio

