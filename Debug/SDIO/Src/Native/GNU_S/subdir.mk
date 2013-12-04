################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SDIO/Src/Native/GNU_S/tinyclr_vector.cpp 

OBJS += \
./SDIO/Src/Native/GNU_S/tinyclr_vector.o 

CPP_DEPS += \
./SDIO/Src/Native/GNU_S/tinyclr_vector.d 


# Each subdirectory must supply rules for building sources it contributes
SDIO/Src/Native/GNU_S/%.o: ../SDIO/Src/Native/GNU_S/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


