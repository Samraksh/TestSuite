################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../NOR/Src/Native/Level_0C/EntryPoint.cpp \
../NOR/Src/Native/Level_0C/NOR.cpp 

C_SRCS += \
../NOR/Src/Native/Level_0C/signalr.c \
../NOR/Src/Native/Level_0C/syscalls.c \
../NOR/Src/Native/Level_0C/test.c 

OBJS += \
./NOR/Src/Native/Level_0C/EntryPoint.o \
./NOR/Src/Native/Level_0C/NOR.o \
./NOR/Src/Native/Level_0C/signalr.o \
./NOR/Src/Native/Level_0C/syscalls.o \
./NOR/Src/Native/Level_0C/test.o 

C_DEPS += \
./NOR/Src/Native/Level_0C/signalr.d \
./NOR/Src/Native/Level_0C/syscalls.d \
./NOR/Src/Native/Level_0C/test.d 

CPP_DEPS += \
./NOR/Src/Native/Level_0C/EntryPoint.d \
./NOR/Src/Native/Level_0C/NOR.d 


# Each subdirectory must supply rules for building sources it contributes
NOR/Src/Native/Level_0C/%.o: ../NOR/Src/Native/Level_0C/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

NOR/Src/Native/Level_0C/%.o: ../NOR/Src/Native/Level_0C/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


