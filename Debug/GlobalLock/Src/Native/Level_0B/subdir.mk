################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GlobalLock/Src/Native/Level_0B/EntryPoint.cpp \
../GlobalLock/Src/Native/Level_0B/GlobalLock.cpp 

OBJS += \
./GlobalLock/Src/Native/Level_0B/EntryPoint.o \
./GlobalLock/Src/Native/Level_0B/GlobalLock.o 

CPP_DEPS += \
./GlobalLock/Src/Native/Level_0B/EntryPoint.d \
./GlobalLock/Src/Native/Level_0B/GlobalLock.d 


# Each subdirectory must supply rules for building sources it contributes
GlobalLock/Src/Native/Level_0B/%.o: ../GlobalLock/Src/Native/Level_0B/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


