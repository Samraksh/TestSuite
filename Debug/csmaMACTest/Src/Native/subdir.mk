################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../csmaMACTest/Src/Native/EntryPoint.cpp \
../csmaMACTest/Src/Native/csmaMACTest.cpp 

C_SRCS += \
../csmaMACTest/Src/Native/signalr.c \
../csmaMACTest/Src/Native/syscalls.c \
../csmaMACTest/Src/Native/test.c 

OBJS += \
./csmaMACTest/Src/Native/EntryPoint.o \
./csmaMACTest/Src/Native/csmaMACTest.o \
./csmaMACTest/Src/Native/signalr.o \
./csmaMACTest/Src/Native/syscalls.o \
./csmaMACTest/Src/Native/test.o 

C_DEPS += \
./csmaMACTest/Src/Native/signalr.d \
./csmaMACTest/Src/Native/syscalls.d \
./csmaMACTest/Src/Native/test.d 

CPP_DEPS += \
./csmaMACTest/Src/Native/EntryPoint.d \
./csmaMACTest/Src/Native/csmaMACTest.d 


# Each subdirectory must supply rules for building sources it contributes
csmaMACTest/Src/Native/%.o: ../csmaMACTest/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

csmaMACTest/Src/Native/%.o: ../csmaMACTest/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


