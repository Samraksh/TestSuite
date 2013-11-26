################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USB/Src/Native/EntryPoint.cpp \
../USB/Src/Native/USB.cpp 

C_SRCS += \
../USB/Src/Native/signalr.c \
../USB/Src/Native/syscalls.c \
../USB/Src/Native/test.c 

OBJS += \
./USB/Src/Native/EntryPoint.o \
./USB/Src/Native/USB.o \
./USB/Src/Native/signalr.o \
./USB/Src/Native/syscalls.o \
./USB/Src/Native/test.o 

C_DEPS += \
./USB/Src/Native/signalr.d \
./USB/Src/Native/syscalls.d \
./USB/Src/Native/test.d 

CPP_DEPS += \
./USB/Src/Native/EntryPoint.d \
./USB/Src/Native/USB.d 


# Each subdirectory must supply rules for building sources it contributes
USB/Src/Native/%.o: ../USB/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

USB/Src/Native/%.o: ../USB/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


