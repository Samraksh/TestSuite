################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SDIO/Src/Native/EntryPoint.cpp \
../SDIO/Src/Native/SDIO.cpp 

C_SRCS += \
../SDIO/Src/Native/signalr.c \
../SDIO/Src/Native/syscalls.c \
../SDIO/Src/Native/test.c 

OBJS += \
./SDIO/Src/Native/EntryPoint.o \
./SDIO/Src/Native/SDIO.o \
./SDIO/Src/Native/signalr.o \
./SDIO/Src/Native/syscalls.o \
./SDIO/Src/Native/test.o 

C_DEPS += \
./SDIO/Src/Native/signalr.d \
./SDIO/Src/Native/syscalls.d \
./SDIO/Src/Native/test.d 

CPP_DEPS += \
./SDIO/Src/Native/EntryPoint.d \
./SDIO/Src/Native/SDIO.d 


# Each subdirectory must supply rules for building sources it contributes
SDIO/Src/Native/%.o: ../SDIO/Src/Native/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDIO/Src/Native/%.o: ../SDIO/Src/Native/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


