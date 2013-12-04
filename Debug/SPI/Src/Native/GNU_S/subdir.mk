################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SPI/Src/Native/GNU_S/tinyclr_vector.cpp 

OBJS += \
./SPI/Src/Native/GNU_S/tinyclr_vector.o 

CPP_DEPS += \
./SPI/Src/Native/GNU_S/tinyclr_vector.d 


# Each subdirectory must supply rules for building sources it contributes
SPI/Src/Native/GNU_S/%.o: ../SPI/Src/Native/GNU_S/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


