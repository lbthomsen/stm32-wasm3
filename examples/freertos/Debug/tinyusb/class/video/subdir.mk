################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/class/video/video_device.c 

OBJS += \
./tinyusb/class/video/video_device.o 

C_DEPS += \
./tinyusb/class/video/video_device.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/class/video/%.o tinyusb/class/video/%.su tinyusb/class/video/%.cyclo: ../tinyusb/class/video/%.c tinyusb/class/video/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lth/src/stm32-wasm3/examples/freertos/wasm3" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/lth/src/stm32-wasm3/examples/freertos/ws2812" -I"/home/lth/src/stm32-wasm3/examples/freertos/tinyusb" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-class-2f-video

clean-tinyusb-2f-class-2f-video:
	-$(RM) ./tinyusb/class/video/video_device.cyclo ./tinyusb/class/video/video_device.d ./tinyusb/class/video/video_device.o ./tinyusb/class/video/video_device.su

.PHONY: clean-tinyusb-2f-class-2f-video

