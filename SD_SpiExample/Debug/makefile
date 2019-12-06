################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SD_SpiExample.axf

# Tool invocations
SD_SpiExample.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Juanma\Documents\MCUXpressoIDE_10.3.1_2233\workspace\LPC_BaseBoardBB1769_R02\Debug" -L"C:\Users\Juanma\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FatFS_SPI\Debug" -L"C:\Users\Juanma\Workspace GIT\lpc_chip_175x_6x\Debug" -Xlinker -Map="SD_SpiExample.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "SD_SpiExample_Debug.ld" -o "SD_SpiExample.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SD_SpiExample.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "SD_SpiExample.axf"; # arm-none-eabi-objcopy -v -O binary "SD_SpiExample.axf" "SD_SpiExample.bin" ; # checksum -p LPC1769 -d "SD_SpiExample.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
