################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ws2812/color_values.c \
../ws2812/ws2812.c \
../ws2812/ws2812_demos.c 

OBJS += \
./ws2812/color_values.o \
./ws2812/ws2812.o \
./ws2812/ws2812_demos.o 

C_DEPS += \
./ws2812/color_values.d \
./ws2812/ws2812.d \
./ws2812/ws2812_demos.d 


# Each subdirectory must supply rules for building sources it contributes
ws2812/%.o ws2812/%.su ws2812/%.cyclo: ../ws2812/%.c ws2812/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Inc -I"/home/lth/src/stm32-wasm3/examples/freertos/wasm3" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/lth/src/stm32-wasm3/examples/freertos/ws2812" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ws2812

clean-ws2812:
	-$(RM) ./ws2812/color_values.cyclo ./ws2812/color_values.d ./ws2812/color_values.o ./ws2812/color_values.su ./ws2812/ws2812.cyclo ./ws2812/ws2812.d ./ws2812/ws2812.o ./ws2812/ws2812.su ./ws2812/ws2812_demos.cyclo ./ws2812/ws2812_demos.d ./ws2812/ws2812_demos.o ./ws2812/ws2812_demos.su

.PHONY: clean-ws2812

