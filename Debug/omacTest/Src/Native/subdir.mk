################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../omacTest/Src/Native/EntryPoint.cpp \
../omacTest/Src/Native/omacTest.cpp 

C_SRCS += \
../omacTest/Src/Native/signalr.c \
../omacTest/Src/Native/syscalls.c \
../omacTest/Src/Native/test.c 

OBJS += \
./omacTest/Src/Native/EntryPoint.o \
./omacTest/Src/Native/omacTest.o \
./omacTest/Src/Native/signalr.o \
./omacTest/Src/Native/syscalls.o \
./omacTest/Src/Native/test.o 

C_DEPS += \
./omacTest/Src/Native/signalr.d \
./omacTest/Src/Native/syscalls.d \
./omacTest/Src/Native/test.d 

CPP_DEPS += \
./omacTest/Src/Native/EntryPoint.d \
./omacTest/Src/Native/omacTest.d 


# Each subdirectory must supply rules for building sources it contributes
omacTest/Src/Native/%.o: ../omacTest/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

omacTest/Src/Native/%.o: ../omacTest/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


