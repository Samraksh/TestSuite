################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Radio/Src/Native/ChangeTxPower/Master/Level_0A/EntryPoint.cpp \
../Radio/Src/Native/ChangeTxPower/Master/Level_0A/Radio.cpp 

C_SRCS += \
../Radio/Src/Native/ChangeTxPower/Master/Level_0A/signalr.c \
../Radio/Src/Native/ChangeTxPower/Master/Level_0A/syscalls.c \
../Radio/Src/Native/ChangeTxPower/Master/Level_0A/test.c 

OBJS += \
./Radio/Src/Native/ChangeTxPower/Master/Level_0A/EntryPoint.o \
./Radio/Src/Native/ChangeTxPower/Master/Level_0A/Radio.o \
./Radio/Src/Native/ChangeTxPower/Master/Level_0A/signalr.o \
./Radio/Src/Native/ChangeTxPower/Master/Level_0A/syscalls.o \
./Radio/Src/Native/ChangeTxPower/Master/Level_0A/test.o 

C_DEPS += \
./Radio/Src/Native/ChangeTxPower/Master/Level_0A/signalr.d \
./Radio/Src/Native/ChangeTxPower/Master/Level_0A/syscalls.d \
./Radio/Src/Native/ChangeTxPower/Master/Level_0A/test.d 

CPP_DEPS += \
./Radio/Src/Native/ChangeTxPower/Master/Level_0A/EntryPoint.d \
./Radio/Src/Native/ChangeTxPower/Master/Level_0A/Radio.d 


# Each subdirectory must supply rules for building sources it contributes
Radio/Src/Native/ChangeTxPower/Master/Level_0A/%.o: ../Radio/Src/Native/ChangeTxPower/Master/Level_0A/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Radio/Src/Native/ChangeTxPower/Master/Level_0A/%.o: ../Radio/Src/Native/ChangeTxPower/Master/Level_0A/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

