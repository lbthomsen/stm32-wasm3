################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/portable/microchip/samx7x/dcd_samx7x.c 

OBJS += \
./tinyusb/portable/microchip/samx7x/dcd_samx7x.o 

C_DEPS += \
./tinyusb/portable/microchip/samx7x/dcd_samx7x.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/portable/microchip/samx7x/%.o tinyusb/portable/microchip/samx7x/%.su tinyusb/portable/microchip/samx7x/%.cyclo: ../tinyusb/portable/microchip/samx7x/%.c tinyusb/portable/microchip/samx7x/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lth/src/stm32-wasm3/examples/dfu/wasm3" -I"/home/lth/src/stm32-wasm3/examples/dfu/ws2812" -I"/home/lth/src/stm32-wasm3/examples/dfu/tinyusb" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-portable-2f-microchip-2f-samx7x

clean-tinyusb-2f-portable-2f-microchip-2f-samx7x:
	-$(RM) ./tinyusb/portable/microchip/samx7x/dcd_samx7x.cyclo ./tinyusb/portable/microchip/samx7x/dcd_samx7x.d ./tinyusb/portable/microchip/samx7x/dcd_samx7x.o ./tinyusb/portable/microchip/samx7x/dcd_samx7x.su

.PHONY: clean-tinyusb-2f-portable-2f-microchip-2f-samx7x

