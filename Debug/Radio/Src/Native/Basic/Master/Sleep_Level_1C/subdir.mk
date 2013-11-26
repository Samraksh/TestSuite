################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Radio/Src/Native/Basic/Master/Sleep_Level_1C/EntryPoint.cpp \
../Radio/Src/Native/Basic/Master/Sleep_Level_1C/Radio.cpp 

C_SRCS += \
../Radio/Src/Native/Basic/Master/Sleep_Level_1C/signalr.c \
../Radio/Src/Native/Basic/Master/Sleep_Level_1C/syscalls.c \
../Radio/Src/Native/Basic/Master/Sleep_Level_1C/test.c 

OBJS += \
./Radio/Src/Native/Basic/Master/Sleep_Level_1C/EntryPoint.o \
./Radio/Src/Native/Basic/Master/Sleep_Level_1C/Radio.o \
./Radio/Src/Native/Basic/Master/Sleep_Level_1C/signalr.o \
./Radio/Src/Native/Basic/Master/Sleep_Level_1C/syscalls.o \
./Radio/Src/Native/Basic/Master/Sleep_Level_1C/test.o 

C_DEPS += \
./Radio/Src/Native/Basic/Master/Sleep_Level_1C/signalr.d \
./Radio/Src/Native/Basic/Master/Sleep_Level_1C/syscalls.d \
./Radio/Src/Native/Basic/Master/Sleep_Level_1C/test.d 

CPP_DEPS += \
./Radio/Src/Native/Basic/Master/Sleep_Level_1C/EntryPoint.d \
./Radio/Src/Native/Basic/Master/Sleep_Level_1C/Radio.d 


# Each subdirectory must supply rules for building sources it contributes
Radio/Src/Native/Basic/Master/Sleep_Level_1C/%.o: ../Radio/Src/Native/Basic/Master/Sleep_Level_1C/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Radio/Src/Native/Basic/Master/Sleep_Level_1C/%.o: ../Radio/Src/Native/Basic/Master/Sleep_Level_1C/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


