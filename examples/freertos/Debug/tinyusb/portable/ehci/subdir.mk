################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/portable/ehci/ehci.c 

OBJS += \
./tinyusb/portable/ehci/ehci.o 

C_DEPS += \
./tinyusb/portable/ehci/ehci.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/portable/ehci/%.o tinyusb/portable/ehci/%.su tinyusb/portable/ehci/%.cyclo: ../tinyusb/portable/ehci/%.c tinyusb/portable/ehci/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -Dd_m3MaxLinearMemoryPages=32 -Dd_m3MaxStackSize=128 -Dd_m3CodePageSize=256 -Dd_m3CodePageAlignSize=64 -Dd_m3MaxFunctionStackHeight=32 -Dd_m3CascadedOpcodes=0 -Dd_m3FixedHeap=65536 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lth/src/stm32-wasm3/examples/freertos/wasm3" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/lth/src/stm32-wasm3/examples/freertos/ws2812" -I"/home/lth/src/stm32-wasm3/examples/freertos/tinyusb" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-portable-2f-ehci

clean-tinyusb-2f-portable-2f-ehci:
	-$(RM) ./tinyusb/portable/ehci/ehci.cyclo ./tinyusb/portable/ehci/ehci.d ./tinyusb/portable/ehci/ehci.o ./tinyusb/portable/ehci/ehci.su

.PHONY: clean-tinyusb-2f-portable-2f-ehci

