################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Timer/Src/Native/RollOverBug/EntryPoint.cpp \
../Timer/Src/Native/RollOverBug/Timer.cpp 

OBJS += \
./Timer/Src/Native/RollOverBug/EntryPoint.o \
./Timer/Src/Native/RollOverBug/Timer.o 

CPP_DEPS += \
./Timer/Src/Native/RollOverBug/EntryPoint.d \
./Timer/Src/Native/RollOverBug/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
Timer/Src/Native/RollOverBug/%.o: ../Timer/Src/Native/RollOverBug/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


