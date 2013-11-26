################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Power/Src/Native/EntryPoint.cpp \
../Power/Src/Native/Power.cpp 

C_SRCS += \
../Power/Src/Native/signalr.c \
../Power/Src/Native/syscalls.c \
../Power/Src/Native/test.c 

OBJS += \
./Power/Src/Native/EntryPoint.o \
./Power/Src/Native/Power.o \
./Power/Src/Native/signalr.o \
./Power/Src/Native/syscalls.o \
./Power/Src/Native/test.o 

C_DEPS += \
./Power/Src/Native/signalr.d \
./Power/Src/Native/syscalls.d \
./Power/Src/Native/test.d 

CPP_DEPS += \
./Power/Src/Native/EntryPoint.d \
./Power/Src/Native/Power.d 


# Each subdirectory must supply rules for building sources it contributes
Power/Src/Native/%.o: ../Power/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Power/Src/Native/%.o: ../Power/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


