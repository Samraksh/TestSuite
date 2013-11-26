################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Radio/Src/Native/Basic/Slave/EntryPoint.cpp \
../Radio/Src/Native/Basic/Slave/Radio.cpp 

C_SRCS += \
../Radio/Src/Native/Basic/Slave/signalr.c \
../Radio/Src/Native/Basic/Slave/syscalls.c \
../Radio/Src/Native/Basic/Slave/test.c 

OBJS += \
./Radio/Src/Native/Basic/Slave/EntryPoint.o \
./Radio/Src/Native/Basic/Slave/Radio.o \
./Radio/Src/Native/Basic/Slave/signalr.o \
./Radio/Src/Native/Basic/Slave/syscalls.o \
./Radio/Src/Native/Basic/Slave/test.o 

C_DEPS += \
./Radio/Src/Native/Basic/Slave/signalr.d \
./Radio/Src/Native/Basic/Slave/syscalls.d \
./Radio/Src/Native/Basic/Slave/test.d 

CPP_DEPS += \
./Radio/Src/Native/Basic/Slave/EntryPoint.d \
./Radio/Src/Native/Basic/Slave/Radio.d 


# Each subdirectory must supply rules for building sources it contributes
Radio/Src/Native/Basic/Slave/%.o: ../Radio/Src/Native/Basic/Slave/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Radio/Src/Native/Basic/Slave/%.o: ../Radio/Src/Native/Basic/Slave/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


