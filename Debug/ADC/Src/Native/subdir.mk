################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ADC/Src/Native/ADC.cpp \
../ADC/Src/Native/EntryPoint.cpp 

C_SRCS += \
../ADC/Src/Native/signalr.c \
../ADC/Src/Native/syscalls.c \
../ADC/Src/Native/test.c 

OBJS += \
./ADC/Src/Native/ADC.o \
./ADC/Src/Native/EntryPoint.o \
./ADC/Src/Native/signalr.o \
./ADC/Src/Native/syscalls.o \
./ADC/Src/Native/test.o 

C_DEPS += \
./ADC/Src/Native/signalr.d \
./ADC/Src/Native/syscalls.d \
./ADC/Src/Native/test.d 

CPP_DEPS += \
./ADC/Src/Native/ADC.d \
./ADC/Src/Native/EntryPoint.d 


# Each subdirectory must supply rules for building sources it contributes
ADC/Src/Native/%.o: ../ADC/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ADC/Src/Native/%.o: ../ADC/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


