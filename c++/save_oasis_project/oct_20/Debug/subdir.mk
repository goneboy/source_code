################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
../dumpRecord.cxx \
../oasisHandle.cxx \
../oasisLibrary.cxx 

CXX_DEPS += \
./dumpRecord.d \
./oasisHandle.d \
./oasisLibrary.d 

OBJS += \
./dumpRecord.o \
./oasisHandle.o \
./oasisLibrary.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


