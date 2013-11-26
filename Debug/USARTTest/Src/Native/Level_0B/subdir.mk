################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USARTTest/Src/Native/Level_0B/EntryPoint.cpp \
../USARTTest/Src/Native/Level_0B/USARTTest.cpp 

C_SRCS += \
../USARTTest/Src/Native/Level_0B/signalr.c \
../USARTTest/Src/Native/Level_0B/syscalls.c \
../USARTTest/Src/Native/Level_0B/test.c 

OBJS += \
./USARTTest/Src/Native/Level_0B/EntryPoint.o \
./USARTTest/Src/Native/Level_0B/USARTTest.o \
./USARTTest/Src/Native/Level_0B/signalr.o \
./USARTTest/Src/Native/Level_0B/syscalls.o \
./USARTTest/Src/Native/Level_0B/test.o 

C_DEPS += \
./USARTTest/Src/Native/Level_0B/signalr.d \
./USARTTest/Src/Native/Level_0B/syscalls.d \
./USARTTest/Src/Native/Level_0B/test.d 

CPP_DEPS += \
./USARTTest/Src/Native/Level_0B/EntryPoint.d \
./USARTTest/Src/Native/Level_0B/USARTTest.d 


# Each subdirectory must supply rules for building sources it contributes
USARTTest/Src/Native/Level_0B/%.o: ../USARTTest/Src/Native/Level_0B/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

USARTTest/Src/Native/Level_0B/%.o: ../USARTTest/Src/Native/Level_0B/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


