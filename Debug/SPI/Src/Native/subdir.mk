################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SPI/Src/Native/EntryPoint.cpp \
../SPI/Src/Native/SPI.cpp 

C_SRCS += \
../SPI/Src/Native/signalr.c \
../SPI/Src/Native/syscalls.c \
../SPI/Src/Native/test.c 

OBJS += \
./SPI/Src/Native/EntryPoint.o \
./SPI/Src/Native/SPI.o \
./SPI/Src/Native/signalr.o \
./SPI/Src/Native/syscalls.o \
./SPI/Src/Native/test.o 

C_DEPS += \
./SPI/Src/Native/signalr.d \
./SPI/Src/Native/syscalls.d \
./SPI/Src/Native/test.d 

CPP_DEPS += \
./SPI/Src/Native/EntryPoint.d \
./SPI/Src/Native/SPI.d 


# Each subdirectory must supply rules for building sources it contributes
SPI/Src/Native/%.o: ../SPI/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SPI/Src/Native/%.o: ../SPI/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


