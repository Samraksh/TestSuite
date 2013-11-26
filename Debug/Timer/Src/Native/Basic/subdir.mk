################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Timer/Src/Native/Basic/EntryPoint.cpp \
../Timer/Src/Native/Basic/Timer.cpp 

OBJS += \
./Timer/Src/Native/Basic/EntryPoint.o \
./Timer/Src/Native/Basic/Timer.o 

CPP_DEPS += \
./Timer/Src/Native/Basic/EntryPoint.d \
./Timer/Src/Native/Basic/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
Timer/Src/Native/Basic/%.o: ../Timer/Src/Native/Basic/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


