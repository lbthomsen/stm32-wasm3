################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/portable/synopsys/dwc2/dcd_dwc2.c \
../tinyusb/portable/synopsys/dwc2/dwc2_common.c \
../tinyusb/portable/synopsys/dwc2/hcd_dwc2.c 

OBJS += \
./tinyusb/portable/synopsys/dwc2/dcd_dwc2.o \
./tinyusb/portable/synopsys/dwc2/dwc2_common.o \
./tinyusb/portable/synopsys/dwc2/hcd_dwc2.o 

C_DEPS += \
./tinyusb/portable/synopsys/dwc2/dcd_dwc2.d \
./tinyusb/portable/synopsys/dwc2/dwc2_common.d \
./tinyusb/portable/synopsys/dwc2/hcd_dwc2.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/portable/synopsys/dwc2/%.o tinyusb/portable/synopsys/dwc2/%.su tinyusb/portable/synopsys/dwc2/%.cyclo: ../tinyusb/portable/synopsys/dwc2/%.c tinyusb/portable/synopsys/dwc2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lth/src/stm32-wasm3/examples/freertos/wasm3" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/lth/src/stm32-wasm3/examples/freertos/ws2812" -I"/home/lth/src/stm32-wasm3/examples/freertos/tinyusb" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-portable-2f-synopsys-2f-dwc2

clean-tinyusb-2f-portable-2f-synopsys-2f-dwc2:
	-$(RM) ./tinyusb/portable/synopsys/dwc2/dcd_dwc2.cyclo ./tinyusb/portable/synopsys/dwc2/dcd_dwc2.d ./tinyusb/portable/synopsys/dwc2/dcd_dwc2.o ./tinyusb/portable/synopsys/dwc2/dcd_dwc2.su ./tinyusb/portable/synopsys/dwc2/dwc2_common.cyclo ./tinyusb/portable/synopsys/dwc2/dwc2_common.d ./tinyusb/portable/synopsys/dwc2/dwc2_common.o ./tinyusb/portable/synopsys/dwc2/dwc2_common.su ./tinyusb/portable/synopsys/dwc2/hcd_dwc2.cyclo ./tinyusb/portable/synopsys/dwc2/hcd_dwc2.d ./tinyusb/portable/synopsys/dwc2/hcd_dwc2.o ./tinyusb/portable/synopsys/dwc2/hcd_dwc2.su

.PHONY: clean-tinyusb-2f-portable-2f-synopsys-2f-dwc2

