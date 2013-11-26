################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GlobalLock/Src/Native/Level_0A/EntryPoint.cpp \
../GlobalLock/Src/Native/Level_0A/GlobalLock.cpp 

OBJS += \
./GlobalLock/Src/Native/Level_0A/EntryPoint.o \
./GlobalLock/Src/Native/Level_0A/GlobalLock.o 

CPP_DEPS += \
./GlobalLock/Src/Native/Level_0A/EntryPoint.d \
./GlobalLock/Src/Native/Level_0A/GlobalLock.d 


# Each subdirectory must supply rules for building sources it contributes
GlobalLock/Src/Native/Level_0A/%.o: ../GlobalLock/Src/Native/Level_0A/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


