################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Radio/Src/Native/ChangeChannel/Master/Level_1A/EntryPoint.cpp \
../Radio/Src/Native/ChangeChannel/Master/Level_1A/Radio.cpp 

C_SRCS += \
../Radio/Src/Native/ChangeChannel/Master/Level_1A/signalr.c \
../Radio/Src/Native/ChangeChannel/Master/Level_1A/syscalls.c \
../Radio/Src/Native/ChangeChannel/Master/Level_1A/test.c 

OBJS += \
./Radio/Src/Native/ChangeChannel/Master/Level_1A/EntryPoint.o \
./Radio/Src/Native/ChangeChannel/Master/Level_1A/Radio.o \
./Radio/Src/Native/ChangeChannel/Master/Level_1A/signalr.o \
./Radio/Src/Native/ChangeChannel/Master/Level_1A/syscalls.o \
./Radio/Src/Native/ChangeChannel/Master/Level_1A/test.o 

C_DEPS += \
./Radio/Src/Native/ChangeChannel/Master/Level_1A/signalr.d \
./Radio/Src/Native/ChangeChannel/Master/Level_1A/syscalls.d \
./Radio/Src/Native/ChangeChannel/Master/Level_1A/test.d 

CPP_DEPS += \
./Radio/Src/Native/ChangeChannel/Master/Level_1A/EntryPoint.d \
./Radio/Src/Native/ChangeChannel/Master/Level_1A/Radio.d 


# Each subdirectory must supply rules for building sources it contributes
Radio/Src/Native/ChangeChannel/Master/Level_1A/%.o: ../Radio/Src/Native/ChangeChannel/Master/Level_1A/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Radio/Src/Native/ChangeChannel/Master/Level_1A/%.o: ../Radio/Src/Native/ChangeChannel/Master/Level_1A/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


