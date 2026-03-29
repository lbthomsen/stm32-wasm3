################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../wasm3/m3_api_libc.c \
../wasm3/m3_api_meta_wasi.c \
../wasm3/m3_api_tracer.c \
../wasm3/m3_api_uvwasi.c \
../wasm3/m3_api_wasi.c \
../wasm3/m3_bind.c \
../wasm3/m3_code.c \
../wasm3/m3_compile.c \
../wasm3/m3_core.c \
../wasm3/m3_env.c \
../wasm3/m3_exec.c \
../wasm3/m3_function.c \
../wasm3/m3_info.c \
../wasm3/m3_module.c \
../wasm3/m3_parse.c 

OBJS += \
./wasm3/m3_api_libc.o \
./wasm3/m3_api_meta_wasi.o \
./wasm3/m3_api_tracer.o \
./wasm3/m3_api_uvwasi.o \
./wasm3/m3_api_wasi.o \
./wasm3/m3_bind.o \
./wasm3/m3_code.o \
./wasm3/m3_compile.o \
./wasm3/m3_core.o \
./wasm3/m3_env.o \
./wasm3/m3_exec.o \
./wasm3/m3_function.o \
./wasm3/m3_info.o \
./wasm3/m3_module.o \
./wasm3/m3_parse.o 

C_DEPS += \
./wasm3/m3_api_libc.d \
./wasm3/m3_api_meta_wasi.d \
./wasm3/m3_api_tracer.d \
./wasm3/m3_api_uvwasi.d \
./wasm3/m3_api_wasi.d \
./wasm3/m3_bind.d \
./wasm3/m3_code.d \
./wasm3/m3_compile.d \
./wasm3/m3_core.d \
./wasm3/m3_env.d \
./wasm3/m3_exec.d \
./wasm3/m3_function.d \
./wasm3/m3_info.d \
./wasm3/m3_module.d \
./wasm3/m3_parse.d 


# Each subdirectory must supply rules for building sources it contributes
wasm3/%.o wasm3/%.su wasm3/%.cyclo: ../wasm3/%.c wasm3/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Inc -I"/home/lth/src/stm32-wasm3/examples/test1/wasm3" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-wasm3

clean-wasm3:
	-$(RM) ./wasm3/m3_api_libc.cyclo ./wasm3/m3_api_libc.d ./wasm3/m3_api_libc.o ./wasm3/m3_api_libc.su ./wasm3/m3_api_meta_wasi.cyclo ./wasm3/m3_api_meta_wasi.d ./wasm3/m3_api_meta_wasi.o ./wasm3/m3_api_meta_wasi.su ./wasm3/m3_api_tracer.cyclo ./wasm3/m3_api_tracer.d ./wasm3/m3_api_tracer.o ./wasm3/m3_api_tracer.su ./wasm3/m3_api_uvwasi.cyclo ./wasm3/m3_api_uvwasi.d ./wasm3/m3_api_uvwasi.o ./wasm3/m3_api_uvwasi.su ./wasm3/m3_api_wasi.cyclo ./wasm3/m3_api_wasi.d ./wasm3/m3_api_wasi.o ./wasm3/m3_api_wasi.su ./wasm3/m3_bind.cyclo ./wasm3/m3_bind.d ./wasm3/m3_bind.o ./wasm3/m3_bind.su ./wasm3/m3_code.cyclo ./wasm3/m3_code.d ./wasm3/m3_code.o ./wasm3/m3_code.su ./wasm3/m3_compile.cyclo ./wasm3/m3_compile.d ./wasm3/m3_compile.o ./wasm3/m3_compile.su ./wasm3/m3_core.cyclo ./wasm3/m3_core.d ./wasm3/m3_core.o ./wasm3/m3_core.su ./wasm3/m3_env.cyclo ./wasm3/m3_env.d ./wasm3/m3_env.o ./wasm3/m3_env.su ./wasm3/m3_exec.cyclo ./wasm3/m3_exec.d ./wasm3/m3_exec.o ./wasm3/m3_exec.su ./wasm3/m3_function.cyclo ./wasm3/m3_function.d ./wasm3/m3_function.o ./wasm3/m3_function.su ./wasm3/m3_info.cyclo ./wasm3/m3_info.d ./wasm3/m3_info.o ./wasm3/m3_info.su ./wasm3/m3_module.cyclo ./wasm3/m3_module.d ./wasm3/m3_module.o ./wasm3/m3_module.su ./wasm3/m3_parse.cyclo ./wasm3/m3_parse.d ./wasm3/m3_parse.o ./wasm3/m3_parse.su

.PHONY: clean-wasm3

