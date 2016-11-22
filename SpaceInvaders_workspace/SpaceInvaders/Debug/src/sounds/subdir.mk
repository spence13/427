################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sounds/soundexplosion.c \
../src/sounds/soundinvade1.c \
../src/sounds/soundinvade2.c \
../src/sounds/soundinvade3.c \
../src/sounds/soundinvade4.c \
../src/sounds/soundinvadekill.c \
../src/sounds/soundshoot.c \
../src/sounds/soundufohigh.c \
../src/sounds/soundufolow.c 

OBJS += \
./src/sounds/soundexplosion.o \
./src/sounds/soundinvade1.o \
./src/sounds/soundinvade2.o \
./src/sounds/soundinvade3.o \
./src/sounds/soundinvade4.o \
./src/sounds/soundinvadekill.o \
./src/sounds/soundshoot.o \
./src/sounds/soundufohigh.o \
./src/sounds/soundufolow.o 

C_DEPS += \
./src/sounds/soundexplosion.d \
./src/sounds/soundinvade1.d \
./src/sounds/soundinvade2.d \
./src/sounds/soundinvade3.d \
./src/sounds/soundinvade4.d \
./src/sounds/soundinvadekill.d \
./src/sounds/soundshoot.d \
./src/sounds/soundufohigh.d \
./src/sounds/soundufolow.d 


# Each subdirectory must supply rules for building sources it contributes
src/sounds/%.o: ../src/sounds/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../hello_world_bsp_0/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


