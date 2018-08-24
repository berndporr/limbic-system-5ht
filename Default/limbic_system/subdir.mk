################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../limbic_system/filter.cpp \
../limbic_system/limbic_system.cpp 

O_SRCS += \
../limbic_system/filter.o \
../limbic_system/limbic_system.o 

OBJS += \
./limbic_system/filter.o \
./limbic_system/limbic_system.o 

CPP_DEPS += \
./limbic_system/filter.d \
./limbic_system/limbic_system.d 


# Each subdirectory must supply rules for building sources it contributes
limbic_system/%.o: ../limbic_system/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


