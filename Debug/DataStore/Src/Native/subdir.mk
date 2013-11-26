################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DataStore/Src/Native/DataStore.cpp \
../DataStore/Src/Native/EntryPoint.cpp 

C_SRCS += \
../DataStore/Src/Native/signalr.c \
../DataStore/Src/Native/syscalls.c \
../DataStore/Src/Native/test.c 

OBJS += \
./DataStore/Src/Native/DataStore.o \
./DataStore/Src/Native/EntryPoint.o \
./DataStore/Src/Native/signalr.o \
./DataStore/Src/Native/syscalls.o \
./DataStore/Src/Native/test.o 

C_DEPS += \
./DataStore/Src/Native/signalr.d \
./DataStore/Src/Native/syscalls.d \
./DataStore/Src/Native/test.d 

CPP_DEPS += \
./DataStore/Src/Native/DataStore.d \
./DataStore/Src/Native/EntryPoint.d 


# Each subdirectory must supply rules for building sources it contributes
DataStore/Src/Native/%.o: ../DataStore/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DataStore/Src/Native/%.o: ../DataStore/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


