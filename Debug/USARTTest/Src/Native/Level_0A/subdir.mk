################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USARTTest/Src/Native/Level_0A/EntryPoint.cpp \
../USARTTest/Src/Native/Level_0A/USARTTest.cpp 

C_SRCS += \
../USARTTest/Src/Native/Level_0A/signalr.c \
../USARTTest/Src/Native/Level_0A/syscalls.c \
../USARTTest/Src/Native/Level_0A/test.c 

OBJS += \
./USARTTest/Src/Native/Level_0A/EntryPoint.o \
./USARTTest/Src/Native/Level_0A/USARTTest.o \
./USARTTest/Src/Native/Level_0A/signalr.o \
./USARTTest/Src/Native/Level_0A/syscalls.o \
./USARTTest/Src/Native/Level_0A/test.o 

C_DEPS += \
./USARTTest/Src/Native/Level_0A/signalr.d \
./USARTTest/Src/Native/Level_0A/syscalls.d \
./USARTTest/Src/Native/Level_0A/test.d 

CPP_DEPS += \
./USARTTest/Src/Native/Level_0A/EntryPoint.d \
./USARTTest/Src/Native/Level_0A/USARTTest.d 


# Each subdirectory must supply rules for building sources it contributes
USARTTest/Src/Native/Level_0A/%.o: ../USARTTest/Src/Native/Level_0A/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

USARTTest/Src/Native/Level_0A/%.o: ../USARTTest/Src/Native/Level_0A/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


