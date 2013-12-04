################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../HALTimerTest/Src/Native/EntryPoint.cpp \
../HALTimerTest/Src/Native/HALTimerTest.cpp 

C_SRCS += \
../HALTimerTest/Src/Native/signalr.c \
../HALTimerTest/Src/Native/syscalls.c \
../HALTimerTest/Src/Native/test.c 

OBJS += \
./HALTimerTest/Src/Native/EntryPoint.o \
./HALTimerTest/Src/Native/HALTimerTest.o \
./HALTimerTest/Src/Native/signalr.o \
./HALTimerTest/Src/Native/syscalls.o \
./HALTimerTest/Src/Native/test.o 

C_DEPS += \
./HALTimerTest/Src/Native/signalr.d \
./HALTimerTest/Src/Native/syscalls.d \
./HALTimerTest/Src/Native/test.d 

CPP_DEPS += \
./HALTimerTest/Src/Native/EntryPoint.d \
./HALTimerTest/Src/Native/HALTimerTest.d 


# Each subdirectory must supply rules for building sources it contributes
HALTimerTest/Src/Native/%.o: ../HALTimerTest/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HALTimerTest/Src/Native/%.o: ../HALTimerTest/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


