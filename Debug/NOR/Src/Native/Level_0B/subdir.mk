################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../NOR/Src/Native/Level_0B/EntryPoint.cpp \
../NOR/Src/Native/Level_0B/NOR.cpp 

C_SRCS += \
../NOR/Src/Native/Level_0B/signalr.c \
../NOR/Src/Native/Level_0B/syscalls.c \
../NOR/Src/Native/Level_0B/test.c 

OBJS += \
./NOR/Src/Native/Level_0B/EntryPoint.o \
./NOR/Src/Native/Level_0B/NOR.o \
./NOR/Src/Native/Level_0B/signalr.o \
./NOR/Src/Native/Level_0B/syscalls.o \
./NOR/Src/Native/Level_0B/test.o 

C_DEPS += \
./NOR/Src/Native/Level_0B/signalr.d \
./NOR/Src/Native/Level_0B/syscalls.d \
./NOR/Src/Native/Level_0B/test.d 

CPP_DEPS += \
./NOR/Src/Native/Level_0B/EntryPoint.d \
./NOR/Src/Native/Level_0B/NOR.d 


# Each subdirectory must supply rules for building sources it contributes
NOR/Src/Native/Level_0B/%.o: ../NOR/Src/Native/Level_0B/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

NOR/Src/Native/Level_0B/%.o: ../NOR/Src/Native/Level_0B/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


