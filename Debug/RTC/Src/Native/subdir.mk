################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../RTC/Src/Native/EntryPoint.cpp \
../RTC/Src/Native/RTC.cpp 

C_SRCS += \
../RTC/Src/Native/signalr.c \
../RTC/Src/Native/syscalls.c \
../RTC/Src/Native/test.c 

OBJS += \
./RTC/Src/Native/EntryPoint.o \
./RTC/Src/Native/RTC.o \
./RTC/Src/Native/signalr.o \
./RTC/Src/Native/syscalls.o \
./RTC/Src/Native/test.o 

C_DEPS += \
./RTC/Src/Native/signalr.d \
./RTC/Src/Native/syscalls.d \
./RTC/Src/Native/test.d 

CPP_DEPS += \
./RTC/Src/Native/EntryPoint.d \
./RTC/Src/Native/RTC.d 


# Each subdirectory must supply rules for building sources it contributes
RTC/Src/Native/%.o: ../RTC/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTC/Src/Native/%.o: ../RTC/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


