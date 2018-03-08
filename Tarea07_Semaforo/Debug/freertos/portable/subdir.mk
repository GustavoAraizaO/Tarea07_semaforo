################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/portable/fsl_tickless_systick.c \
../freertos/portable/heap_4.c \
../freertos/portable/port.c 

OBJS += \
./freertos/portable/fsl_tickless_systick.o \
./freertos/portable/heap_4.o \
./freertos/portable/port.o 

C_DEPS += \
./freertos/portable/fsl_tickless_systick.d \
./freertos/portable/heap_4.d \
./freertos/portable/port.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/portable/%.o: ../freertos/portable/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -DDEBUG -DSDK_OS_FREE_RTOS -DFSL_RTOS_FREE_RTOS -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -I"C:\Projects_2\Tarea07_Semaforo\board" -I"C:\Projects_2\Tarea07_Semaforo\source" -I"C:\Projects_2\Tarea07_Semaforo" -I"C:\Projects_2\Tarea07_Semaforo\freertos\include" -I"C:\Projects_2\Tarea07_Semaforo\freertos\portable" -I"C:\Projects_2\Tarea07_Semaforo\drivers" -I"C:\Projects_2\Tarea07_Semaforo\CMSIS" -I"C:\Projects_2\Tarea07_Semaforo\utilities" -I"C:\Projects_2\Tarea07_Semaforo\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


