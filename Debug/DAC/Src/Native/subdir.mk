################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DAC/Src/Native/DAC.cpp \
../DAC/Src/Native/EntryPoint.cpp 

C_SRCS += \
../DAC/Src/Native/signalr.c \
../DAC/Src/Native/syscalls.c \
../DAC/Src/Native/test.c 

OBJS += \
./DAC/Src/Native/DAC.o \
./DAC/Src/Native/EntryPoint.o \
./DAC/Src/Native/signalr.o \
./DAC/Src/Native/syscalls.o \
./DAC/Src/Native/test.o 

C_DEPS += \
./DAC/Src/Native/signalr.d \
./DAC/Src/Native/syscalls.d \
./DAC/Src/Native/test.d 

CPP_DEPS += \
./DAC/Src/Native/DAC.d \
./DAC/Src/Native/EntryPoint.d 


# Each subdirectory must supply rules for building sources it contributes
DAC/Src/Native/%.o: ../DAC/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DAC/Src/Native/%.o: ../DAC/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


