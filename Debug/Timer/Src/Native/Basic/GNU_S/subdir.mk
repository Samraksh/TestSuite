################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Timer/Src/Native/Basic/GNU_S/tinyclr_vector.cpp 

OBJS += \
./Timer/Src/Native/Basic/GNU_S/tinyclr_vector.o 

CPP_DEPS += \
./Timer/Src/Native/Basic/GNU_S/tinyclr_vector.d 


# Each subdirectory must supply rules for building sources it contributes
Timer/Src/Native/Basic/GNU_S/%.o: ../Timer/Src/Native/Basic/GNU_S/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

