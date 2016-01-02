################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
drivers/rgb.obj: /media/bill/WANG/TivaTM4C123G/TIVAWARE/examples/boards/ek-tm4c123gxl-boostxl-senshub/drivers/rgb.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/bill/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 -g --include_path="/home/bill/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="/media/bill/WANG/TivaTM4C123G/TIVAWARE/examples/boards/ek-tm4c123gxl-boostxl-senshub" --include_path="/media/bill/WANG/TivaTM4C123G/TIVAWARE" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=UART_BUFFERED --diag_warning=225 --display_error_number --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="drivers/rgb.pp" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


