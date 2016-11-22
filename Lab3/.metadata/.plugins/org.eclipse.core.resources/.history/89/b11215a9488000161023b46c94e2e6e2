<<<<<<< HEAD
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
Lab3.elf.size \

ELFCHECK += \
Lab3.elf.elfcheck \


# All Target
all: Lab3.elf secondary-outputs

# Tool invocations
Lab3.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: MicroBlaze gcc linker
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../hello_world_bsp_0/microblaze_0/lib -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -o"Lab3.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo ' '

Lab3.elf.size: Lab3.elf
	@echo Invoking: MicroBlaze Print Size
	mb-size Lab3.elf  |tee "Lab3.elf.size"
	@echo Finished building: $@
	@echo ' '

Lab3.elf.elfcheck: Lab3.elf
	@echo Invoking: Xilinx ELF Check
	elfcheck Lab3.elf -hw ../../SpaceInvadersHW_hw_platform/system.xml -pe microblaze_0  |tee "Lab3.elf.elfcheck"
	@echo Finished building: $@
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) Lab3.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
=======
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
Lab3.elf.size \

ELFCHECK += \
Lab3.elf.elfcheck \


# All Target
all: Lab3.elf secondary-outputs

# Tool invocations
Lab3.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: MicroBlaze gcc linker
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../hello_world_bsp_0/microblaze_0/lib -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -o"Lab3.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo ' '

Lab3.elf.size: Lab3.elf
	@echo Invoking: MicroBlaze Print Size
	mb-size Lab3.elf  |tee "Lab3.elf.size"
	@echo Finished building: $@
	@echo ' '

Lab3.elf.elfcheck: Lab3.elf
	@echo Invoking: Xilinx ELF Check
	elfcheck Lab3.elf -hw ../../SpaceInvadersHW_hw_platform/system.xml -pe microblaze_0  |tee "Lab3.elf.elfcheck"
	@echo Finished building: $@
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) Lab3.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
