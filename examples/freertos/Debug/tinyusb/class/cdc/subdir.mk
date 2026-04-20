################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/class/cdc/cdc_device.c \
../tinyusb/class/cdc/cdc_host.c 

OBJS += \
./tinyusb/class/cdc/cdc_device.o \
./tinyusb/class/cdc/cdc_host.o 

C_DEPS += \
./tinyusb/class/cdc/cdc_device.d \
./tinyusb/class/cdc/cdc_host.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/class/cdc/%.o tinyusb/class/cdc/%.su tinyusb/class/cdc/%.cyclo: ../tinyusb/class/cdc/%.c tinyusb/class/cdc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lth/src/stm32-wasm3/examples/freertos/wasm3" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/lth/src/stm32-wasm3/examples/freertos/ws2812" -I"/home/lth/src/stm32-wasm3/examples/freertos/tinyusb" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-class-2f-cdc

clean-tinyusb-2f-class-2f-cdc:
	-$(RM) ./tinyusb/class/cdc/cdc_device.cyclo ./tinyusb/class/cdc/cdc_device.d ./tinyusb/class/cdc/cdc_device.o ./tinyusb/class/cdc/cdc_device.su ./tinyusb/class/cdc/cdc_host.cyclo ./tinyusb/class/cdc/cdc_host.d ./tinyusb/class/cdc/cdc_host.o ./tinyusb/class/cdc/cdc_host.su

.PHONY: clean-tinyusb-2f-class-2f-cdc

