################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BlockStorageTest/Src/Native/BlockStorageTest.cpp \
../BlockStorageTest/Src/Native/EntryPoint.cpp 

C_SRCS += \
../BlockStorageTest/Src/Native/signalr.c \
../BlockStorageTest/Src/Native/syscalls.c \
../BlockStorageTest/Src/Native/test.c 

OBJS += \
./BlockStorageTest/Src/Native/BlockStorageTest.o \
./BlockStorageTest/Src/Native/EntryPoint.o \
./BlockStorageTest/Src/Native/signalr.o \
./BlockStorageTest/Src/Native/syscalls.o \
./BlockStorageTest/Src/Native/test.o 

C_DEPS += \
./BlockStorageTest/Src/Native/signalr.d \
./BlockStorageTest/Src/Native/syscalls.d \
./BlockStorageTest/Src/Native/test.d 

CPP_DEPS += \
./BlockStorageTest/Src/Native/BlockStorageTest.d \
./BlockStorageTest/Src/Native/EntryPoint.d 


# Each subdirectory must supply rules for building sources it contributes
BlockStorageTest/Src/Native/%.o: ../BlockStorageTest/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

BlockStorageTest/Src/Native/%.o: ../BlockStorageTest/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


