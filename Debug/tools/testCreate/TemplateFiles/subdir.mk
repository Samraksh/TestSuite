################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/testCreate/TemplateFiles/EntryPoint.cpp \
../tools/testCreate/TemplateFiles/temp.cpp \
../tools/testCreate/TemplateFiles/template.cpp \
../tools/testCreate/TemplateFiles/tinyclr_vector.cpp 

C_SRCS += \
../tools/testCreate/TemplateFiles/signalr.c \
../tools/testCreate/TemplateFiles/syscalls.c \
../tools/testCreate/TemplateFiles/test.c 

OBJS += \
./tools/testCreate/TemplateFiles/EntryPoint.o \
./tools/testCreate/TemplateFiles/signalr.o \
./tools/testCreate/TemplateFiles/syscalls.o \
./tools/testCreate/TemplateFiles/temp.o \
./tools/testCreate/TemplateFiles/template.o \
./tools/testCreate/TemplateFiles/test.o \
./tools/testCreate/TemplateFiles/tinyclr_vector.o 

C_DEPS += \
./tools/testCreate/TemplateFiles/signalr.d \
./tools/testCreate/TemplateFiles/syscalls.d \
./tools/testCreate/TemplateFiles/test.d 

CPP_DEPS += \
./tools/testCreate/TemplateFiles/EntryPoint.d \
./tools/testCreate/TemplateFiles/temp.d \
./tools/testCreate/TemplateFiles/template.d \
./tools/testCreate/TemplateFiles/tinyclr_vector.d 


# Each subdirectory must supply rules for building sources it contributes
tools/testCreate/TemplateFiles/%.o: ../tools/testCreate/TemplateFiles/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

tools/testCreate/TemplateFiles/%.o: ../tools/testCreate/TemplateFiles/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


