################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../NOR/Src/Native/Level_1/EntryPoint.cpp \
../NOR/Src/Native/Level_1/NOR.cpp 

C_SRCS += \
../NOR/Src/Native/Level_1/signalr.c \
../NOR/Src/Native/Level_1/syscalls.c \
../NOR/Src/Native/Level_1/test.c 

OBJS += \
./NOR/Src/Native/Level_1/EntryPoint.o \
./NOR/Src/Native/Level_1/NOR.o \
./NOR/Src/Native/Level_1/signalr.o \
./NOR/Src/Native/Level_1/syscalls.o \
./NOR/Src/Native/Level_1/test.o 

C_DEPS += \
./NOR/Src/Native/Level_1/signalr.d \
./NOR/Src/Native/Level_1/syscalls.d \
./NOR/Src/Native/Level_1/test.d 

CPP_DEPS += \
./NOR/Src/Native/Level_1/EntryPoint.d \
./NOR/Src/Native/Level_1/NOR.d 


# Each subdirectory must supply rules for building sources it contributes
NOR/Src/Native/Level_1/%.o: ../NOR/Src/Native/Level_1/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

NOR/Src/Native/Level_1/%.o: ../NOR/Src/Native/Level_1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


