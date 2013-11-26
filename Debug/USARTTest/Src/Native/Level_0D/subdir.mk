################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USARTTest/Src/Native/Level_0D/EntryPoint.cpp \
../USARTTest/Src/Native/Level_0D/USARTTest.cpp 

C_SRCS += \
../USARTTest/Src/Native/Level_0D/signalr.c \
../USARTTest/Src/Native/Level_0D/syscalls.c \
../USARTTest/Src/Native/Level_0D/test.c 

OBJS += \
./USARTTest/Src/Native/Level_0D/EntryPoint.o \
./USARTTest/Src/Native/Level_0D/USARTTest.o \
./USARTTest/Src/Native/Level_0D/signalr.o \
./USARTTest/Src/Native/Level_0D/syscalls.o \
./USARTTest/Src/Native/Level_0D/test.o 

C_DEPS += \
./USARTTest/Src/Native/Level_0D/signalr.d \
./USARTTest/Src/Native/Level_0D/syscalls.d \
./USARTTest/Src/Native/Level_0D/test.d 

CPP_DEPS += \
./USARTTest/Src/Native/Level_0D/EntryPoint.d \
./USARTTest/Src/Native/Level_0D/USARTTest.d 


# Each subdirectory must supply rules for building sources it contributes
USARTTest/Src/Native/Level_0D/%.o: ../USARTTest/Src/Native/Level_0D/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

USARTTest/Src/Native/Level_0D/%.o: ../USARTTest/Src/Native/Level_0D/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


