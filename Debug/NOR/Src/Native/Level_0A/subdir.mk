################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../NOR/Src/Native/Level_0A/EntryPoint.cpp \
../NOR/Src/Native/Level_0A/NOR.cpp 

C_SRCS += \
../NOR/Src/Native/Level_0A/signalr.c \
../NOR/Src/Native/Level_0A/syscalls.c \
../NOR/Src/Native/Level_0A/test.c 

OBJS += \
./NOR/Src/Native/Level_0A/EntryPoint.o \
./NOR/Src/Native/Level_0A/NOR.o \
./NOR/Src/Native/Level_0A/signalr.o \
./NOR/Src/Native/Level_0A/syscalls.o \
./NOR/Src/Native/Level_0A/test.o 

C_DEPS += \
./NOR/Src/Native/Level_0A/signalr.d \
./NOR/Src/Native/Level_0A/syscalls.d \
./NOR/Src/Native/Level_0A/test.d 

CPP_DEPS += \
./NOR/Src/Native/Level_0A/EntryPoint.d \
./NOR/Src/Native/Level_0A/NOR.d 


# Each subdirectory must supply rules for building sources it contributes
NOR/Src/Native/Level_0A/%.o: ../NOR/Src/Native/Level_0A/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

NOR/Src/Native/Level_0A/%.o: ../NOR/Src/Native/Level_0A/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


