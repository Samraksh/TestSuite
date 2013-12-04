################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Radio/Src/Native/ChangeChannel/Slave/EntryPoint.cpp \
../Radio/Src/Native/ChangeChannel/Slave/Radio.cpp 

C_SRCS += \
../Radio/Src/Native/ChangeChannel/Slave/signalr.c \
../Radio/Src/Native/ChangeChannel/Slave/syscalls.c \
../Radio/Src/Native/ChangeChannel/Slave/test.c 

OBJS += \
./Radio/Src/Native/ChangeChannel/Slave/EntryPoint.o \
./Radio/Src/Native/ChangeChannel/Slave/Radio.o \
./Radio/Src/Native/ChangeChannel/Slave/signalr.o \
./Radio/Src/Native/ChangeChannel/Slave/syscalls.o \
./Radio/Src/Native/ChangeChannel/Slave/test.o 

C_DEPS += \
./Radio/Src/Native/ChangeChannel/Slave/signalr.d \
./Radio/Src/Native/ChangeChannel/Slave/syscalls.d \
./Radio/Src/Native/ChangeChannel/Slave/test.d 

CPP_DEPS += \
./Radio/Src/Native/ChangeChannel/Slave/EntryPoint.d \
./Radio/Src/Native/ChangeChannel/Slave/Radio.d 


# Each subdirectory must supply rules for building sources it contributes
Radio/Src/Native/ChangeChannel/Slave/%.o: ../Radio/Src/Native/ChangeChannel/Slave/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Radio/Src/Native/ChangeChannel/Slave/%.o: ../Radio/Src/Native/ChangeChannel/Slave/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


