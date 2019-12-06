################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SD_SpiExample.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/sysinit.c 

OBJS += \
./src/SD_SpiExample.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/sysinit.o 

C_DEPS += \
./src/SD_SpiExample.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\Juanma\Documents\MCUXpressoIDE_10.3.1_2233\workspace\SD_SpiExample\inc" -I"C:\Users\Juanma\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FatFS_SPI\inc" -I"C:\Users\Juanma\Documents\MCUXpressoIDE_10.3.1_2233\workspace\LPC_BaseBoardBB1769_R02\inc" -I"C:\Users\Juanma\Workspace GIT\lpc_chip_175x_6x\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


