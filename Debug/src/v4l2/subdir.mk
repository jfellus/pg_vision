################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/v4l2/V4LIn.cpp 

OBJS += \
./src/v4l2/V4LIn.o 

CPP_DEPS += \
./src/v4l2/V4LIn.d 


# Each subdirectory must supply rules for building sources it contributes
src/v4l2/%.o: ../src/v4l2/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include -I/usr/include/x86_64-linux-gnu -I/home/jfellus/.pgcc/includes -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


