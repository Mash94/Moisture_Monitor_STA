################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ff.c \
../src/mmc.c \
../src/rtc.c 

OBJS += \
./src/ff.o \
./src/mmc.o \
./src/rtc.o 

C_DEPS += \
./src/ff.d \
./src/mmc.d \
./src/rtc.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -D__LPC17XX__ -I"C:\Users\Juanma\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FatFS_SPI\inc" -I"C:\Users\Juanma\Documents\MCUXpressoIDE_10.3.1_2233\workspace\LPC_BaseBoardBB1769_R02\inc" -I"C:\Users\Juanma\Workspace GIT\lpc_chip_175x_6x\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


