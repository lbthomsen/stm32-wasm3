################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/portable/st/stm32_fsdev/dcd_stm32_fsdev.c \
../tinyusb/portable/st/stm32_fsdev/fsdev_common.c \
../tinyusb/portable/st/stm32_fsdev/hcd_stm32_fsdev.c 

OBJS += \
./tinyusb/portable/st/stm32_fsdev/dcd_stm32_fsdev.o \
./tinyusb/portable/st/stm32_fsdev/fsdev_common.o \
./tinyusb/portable/st/stm32_fsdev/hcd_stm32_fsdev.o 

C_DEPS += \
./tinyusb/portable/st/stm32_fsdev/dcd_stm32_fsdev.d \
./tinyusb/portable/st/stm32_fsdev/fsdev_common.d \
./tinyusb/portable/st/stm32_fsdev/hcd_stm32_fsdev.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/portable/st/stm32_fsdev/%.o tinyusb/portable/st/stm32_fsdev/%.su tinyusb/portable/st/stm32_fsdev/%.cyclo: ../tinyusb/portable/st/stm32_fsdev/%.c tinyusb/portable/st/stm32_fsdev/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -Dd_m3MaxLinearMemoryPages=32 -Dd_m3MaxStackSize=128 -Dd_m3CodePageSize=256 -Dd_m3CodePageAlignSize=64 -Dd_m3MaxFunctionStackHeight=32 -Dd_m3CascadedOpcodes=0 -Dd_m3FixedHeap=65536 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lth/src/stm32-wasm3/examples/freertos/wasm3" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/lth/src/stm32-wasm3/examples/freertos/ws2812" -I"/home/lth/src/stm32-wasm3/examples/freertos/tinyusb" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-portable-2f-st-2f-stm32_fsdev

clean-tinyusb-2f-portable-2f-st-2f-stm32_fsdev:
	-$(RM) ./tinyusb/portable/st/stm32_fsdev/dcd_stm32_fsdev.cyclo ./tinyusb/portable/st/stm32_fsdev/dcd_stm32_fsdev.d ./tinyusb/portable/st/stm32_fsdev/dcd_stm32_fsdev.o ./tinyusb/portable/st/stm32_fsdev/dcd_stm32_fsdev.su ./tinyusb/portable/st/stm32_fsdev/fsdev_common.cyclo ./tinyusb/portable/st/stm32_fsdev/fsdev_common.d ./tinyusb/portable/st/stm32_fsdev/fsdev_common.o ./tinyusb/portable/st/stm32_fsdev/fsdev_common.su ./tinyusb/portable/st/stm32_fsdev/hcd_stm32_fsdev.cyclo ./tinyusb/portable/st/stm32_fsdev/hcd_stm32_fsdev.d ./tinyusb/portable/st/stm32_fsdev/hcd_stm32_fsdev.o ./tinyusb/portable/st/stm32_fsdev/hcd_stm32_fsdev.su

.PHONY: clean-tinyusb-2f-portable-2f-st-2f-stm32_fsdev

