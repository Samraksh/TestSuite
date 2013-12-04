################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USARTTest/Src/Native/Level_0C/EntryPoint.cpp \
../USARTTest/Src/Native/Level_0C/USARTTest.cpp 

C_SRCS += \
../USARTTest/Src/Native/Level_0C/signalr.c \
../USARTTest/Src/Native/Level_0C/syscalls.c \
../USARTTest/Src/Native/Level_0C/test.c 

OBJS += \
./USARTTest/Src/Native/Level_0C/EntryPoint.o \
./USARTTest/Src/Native/Level_0C/USARTTest.o \
./USARTTest/Src/Native/Level_0C/signalr.o \
./USARTTest/Src/Native/Level_0C/syscalls.o \
./USARTTest/Src/Native/Level_0C/test.o 

C_DEPS += \
./USARTTest/Src/Native/Level_0C/signalr.d \
./USARTTest/Src/Native/Level_0C/syscalls.d \
./USARTTest/Src/Native/Level_0C/test.d 

CPP_DEPS += \
./USARTTest/Src/Native/Level_0C/EntryPoint.d \
./USARTTest/Src/Native/Level_0C/USARTTest.d 


# Each subdirectory must supply rules for building sources it contributes
USARTTest/Src/Native/Level_0C/%.o: ../USARTTest/Src/Native/Level_0C/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

USARTTest/Src/Native/Level_0C/%.o: ../USARTTest/Src/Native/Level_0C/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


