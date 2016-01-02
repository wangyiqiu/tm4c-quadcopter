################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ble_mini.obj" \
"./compdcm_mpu9150.obj" \
"./control.obj" \
"./esc.obj" \
"./main.obj" \
"./pid.obj" \
"./startup_ccs.obj" \
"./utils/uartstdio.obj" \
"../compdcm_mpu9150_ccs.cmd" \
$(GEN_CMDS__FLAG) \
-l"libc.a" \
-l"/Users/billwang/Documents/TIVAWARE/driverlib/ccs/Debug/driverlib.lib" \
-l"/Users/billwang/Documents/TIVAWARE/sensorlib/ccs/Debug/sensorlib.lib" \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include utils/subdir_vars.mk
-include subdir_rules.mk
-include utils/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
tm4c_quadcopter.out \

EXE_OUTPUTS__QUOTED += \
"tm4c_quadcopter.out" \


# All Target
all: tm4c_quadcopter.out

# Tool invocations
tm4c_quadcopter.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=UART_BUFFERED --diag_warning=225 --display_error_number --diag_wrap=off --gen_func_subsections=on --ual -z --stack_size=1024 -m"compdcm_mpu9150_ccs.map" --heap_size=0 -i"/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/lib" -i"/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="tm4c_quadcopter_linkInfo.xml" --rom_model -o "tm4c_quadcopter.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "ble_mini.obj" "compdcm_mpu9150.obj" "control.obj" "esc.obj" "main.obj" "pid.obj" "startup_ccs.obj" "utils/uartstdio.obj" 
	-$(RM) "ble_mini.pp" "compdcm_mpu9150.pp" "control.pp" "esc.pp" "main.pp" "pid.pp" "startup_ccs.pp" "utils/uartstdio.pp" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"/Applications/ti/ccsv6/utils/tiobj2bin/tiobj2bin" "tm4c_quadcopter.out" "tm4c_quadcopter.bin" "/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armofd" "/Applications/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armhex" "/Applications/ti/ccsv6/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

