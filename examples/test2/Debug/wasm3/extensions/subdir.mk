################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../wasm3/extensions/m3_extensions.c 

OBJS += \
./wasm3/extensions/m3_extensions.o 

C_DEPS += \
./wasm3/extensions/m3_extensions.d 


# Each subdirectory must supply rules for building sources it contributes
wasm3/extensions/%.o wasm3/extensions/%.su wasm3/extensions/%.cyclo: ../wasm3/extensions/%.c wasm3/extensions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Inc -I"/home/lth/src/stm32-wasm3/examples/test2/wasm3" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-wasm3-2f-extensions

clean-wasm3-2f-extensions:
	-$(RM) ./wasm3/extensions/m3_extensions.cyclo ./wasm3/extensions/m3_extensions.d ./wasm3/extensions/m3_extensions.o ./wasm3/extensions/m3_extensions.su

.PHONY: clean-wasm3-2f-extensions

