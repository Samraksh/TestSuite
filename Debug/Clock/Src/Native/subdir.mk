################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Clock/Src/Native/Clock.cpp \
../Clock/Src/Native/EntryPoint.cpp 

C_SRCS += \
../Clock/Src/Native/signalr.c \
../Clock/Src/Native/syscalls.c \
../Clock/Src/Native/test.c 

OBJS += \
./Clock/Src/Native/Clock.o \
./Clock/Src/Native/EntryPoint.o \
./Clock/Src/Native/signalr.o \
./Clock/Src/Native/syscalls.o \
./Clock/Src/Native/test.o 

C_DEPS += \
./Clock/Src/Native/signalr.d \
./Clock/Src/Native/syscalls.d \
./Clock/Src/Native/test.d 

CPP_DEPS += \
./Clock/Src/Native/Clock.d \
./Clock/Src/Native/EntryPoint.d 


# Each subdirectory must supply rules for building sources it contributes
Clock/Src/Native/%.o: ../Clock/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Clock/Src/Native/%.o: ../Clock/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


