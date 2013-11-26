################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Radio/Src/Native/ChangeChannel/Master/Level_0A/GNU_S/tinyclr_vector.cpp 

OBJS += \
./Radio/Src/Native/ChangeChannel/Master/Level_0A/GNU_S/tinyclr_vector.o 

CPP_DEPS += \
./Radio/Src/Native/ChangeChannel/Master/Level_0A/GNU_S/tinyclr_vector.d 


# Each subdirectory must supply rules for building sources it contributes
Radio/Src/Native/ChangeChannel/Master/Level_0A/GNU_S/%.o: ../Radio/Src/Native/ChangeChannel/Master/Level_0A/GNU_S/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


