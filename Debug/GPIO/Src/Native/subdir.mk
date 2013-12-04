################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GPIO/Src/Native/EntryPoint.cpp \
../GPIO/Src/Native/GPIO.cpp 

OBJS += \
./GPIO/Src/Native/EntryPoint.o \
./GPIO/Src/Native/GPIO.o 

CPP_DEPS += \
./GPIO/Src/Native/EntryPoint.d \
./GPIO/Src/Native/GPIO.d 


# Each subdirectory must supply rules for building sources it contributes
GPIO/Src/Native/%.o: ../GPIO/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


