################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../INTC/Src/Native/EntryPoint.cpp \
../INTC/Src/Native/INTC.cpp 

C_SRCS += \
../INTC/Src/Native/signalr.c \
../INTC/Src/Native/syscalls.c \
../INTC/Src/Native/test.c 

OBJS += \
./INTC/Src/Native/EntryPoint.o \
./INTC/Src/Native/INTC.o \
./INTC/Src/Native/signalr.o \
./INTC/Src/Native/syscalls.o \
./INTC/Src/Native/test.o 

C_DEPS += \
./INTC/Src/Native/signalr.d \
./INTC/Src/Native/syscalls.d \
./INTC/Src/Native/test.d 

CPP_DEPS += \
./INTC/Src/Native/EntryPoint.d \
./INTC/Src/Native/INTC.d 


# Each subdirectory must supply rules for building sources it contributes
INTC/Src/Native/%.o: ../INTC/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

INTC/Src/Native/%.o: ../INTC/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


