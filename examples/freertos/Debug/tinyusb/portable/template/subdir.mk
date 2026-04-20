################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/portable/template/dcd_template.c \
../tinyusb/portable/template/hcd_template.c 

OBJS += \
./tinyusb/portable/template/dcd_template.o \
./tinyusb/portable/template/hcd_template.o 

C_DEPS += \
./tinyusb/portable/template/dcd_template.d \
./tinyusb/portable/template/hcd_template.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/portable/template/%.o tinyusb/portable/template/%.su tinyusb/portable/template/%.cyclo: ../tinyusb/portable/template/%.c tinyusb/portable/template/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lth/src/stm32-wasm3/examples/freertos/wasm3" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/lth/src/stm32-wasm3/examples/freertos/ws2812" -I"/home/lth/src/stm32-wasm3/examples/freertos/tinyusb" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-portable-2f-template

clean-tinyusb-2f-portable-2f-template:
	-$(RM) ./tinyusb/portable/template/dcd_template.cyclo ./tinyusb/portable/template/dcd_template.d ./tinyusb/portable/template/dcd_template.o ./tinyusb/portable/template/dcd_template.su ./tinyusb/portable/template/hcd_template.cyclo ./tinyusb/portable/template/hcd_template.d ./tinyusb/portable/template/hcd_template.o ./tinyusb/portable/template/hcd_template.su

.PHONY: clean-tinyusb-2f-portable-2f-template

