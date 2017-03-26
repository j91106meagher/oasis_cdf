################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/oasis_calc_cdf.c \
../src/oasis_cdf.c \
../src/oasis_files.c 

OBJS += \
./src/oasis_calc_cdf.o \
./src/oasis_cdf.o \
./src/oasis_files.o 

C_DEPS += \
./src/oasis_calc_cdf.d \
./src/oasis_cdf.d \
./src/oasis_files.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/local/include/marray -I/usr/local/include/gsl -O0 -g3 -Wall -c -fmessage-length=0 -fopenmp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


