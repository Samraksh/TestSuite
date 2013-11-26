################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SDIO/Src/Native/Level_1/EntryPoint.cpp \
../SDIO/Src/Native/Level_1/SDIO.cpp 

C_SRCS += \
../SDIO/Src/Native/Level_1/signalr.c \
../SDIO/Src/Native/Level_1/syscalls.c \
../SDIO/Src/Native/Level_1/test.c 

OBJS += \
./SDIO/Src/Native/Level_1/EntryPoint.o \
./SDIO/Src/Native/Level_1/SDIO.o \
./SDIO/Src/Native/Level_1/signalr.o \
./SDIO/Src/Native/Level_1/syscalls.o \
./SDIO/Src/Native/Level_1/test.o 

C_DEPS += \
./SDIO/Src/Native/Level_1/signalr.d \
./SDIO/Src/Native/Level_1/syscalls.d \
./SDIO/Src/Native/Level_1/test.d 

CPP_DEPS += \
./SDIO/Src/Native/Level_1/EntryPoint.d \
./SDIO/Src/Native/Level_1/SDIO.d 


# Each subdirectory must supply rules for building sources it contributes
SDIO/Src/Native/Level_1/%.o: ../SDIO/Src/Native/Level_1/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDIO/Src/Native/Level_1/%.o: ../SDIO/Src/Native/Level_1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


