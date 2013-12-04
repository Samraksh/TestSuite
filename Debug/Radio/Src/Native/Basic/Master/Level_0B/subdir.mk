################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Radio/Src/Native/Basic/Master/Level_0B/EntryPoint.cpp \
../Radio/Src/Native/Basic/Master/Level_0B/Radio.cpp 

C_SRCS += \
../Radio/Src/Native/Basic/Master/Level_0B/signalr.c \
../Radio/Src/Native/Basic/Master/Level_0B/syscalls.c \
../Radio/Src/Native/Basic/Master/Level_0B/test.c 

OBJS += \
./Radio/Src/Native/Basic/Master/Level_0B/EntryPoint.o \
./Radio/Src/Native/Basic/Master/Level_0B/Radio.o \
./Radio/Src/Native/Basic/Master/Level_0B/signalr.o \
./Radio/Src/Native/Basic/Master/Level_0B/syscalls.o \
./Radio/Src/Native/Basic/Master/Level_0B/test.o 

C_DEPS += \
./Radio/Src/Native/Basic/Master/Level_0B/signalr.d \
./Radio/Src/Native/Basic/Master/Level_0B/syscalls.d \
./Radio/Src/Native/Basic/Master/Level_0B/test.d 

CPP_DEPS += \
./Radio/Src/Native/Basic/Master/Level_0B/EntryPoint.d \
./Radio/Src/Native/Basic/Master/Level_0B/Radio.d 


# Each subdirectory must supply rules for building sources it contributes
Radio/Src/Native/Basic/Master/Level_0B/%.o: ../Radio/Src/Native/Basic/Master/Level_0B/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Radio/Src/Native/Basic/Master/Level_0B/%.o: ../Radio/Src/Native/Basic/Master/Level_0B/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


