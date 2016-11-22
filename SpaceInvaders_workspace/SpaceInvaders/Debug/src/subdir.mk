<<<<<<< HEAD
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
=======
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
C_SRCS += \
../src/aliens.c \
../src/bullet.c \
../src/bunker.c \
../src/end.c \
../src/main.c \
../src/pit.c \
../src/platform.c \
../src/saucer.c \
../src/sounds.c \
../src/stats.c \
../src/tank.c \
<<<<<<< HEAD
../src/xac97_l.c 

LD_SRCS += \
../src/lscript.ld 

=======
../src/xac97_l.c 

LD_SRCS += \
../src/lscript.ld 

>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
OBJS += \
./src/aliens.o \
./src/bullet.o \
./src/bunker.o \
./src/end.o \
./src/main.o \
./src/pit.o \
./src/platform.o \
./src/saucer.o \
./src/sounds.o \
./src/stats.o \
./src/tank.o \
<<<<<<< HEAD
./src/xac97_l.o 

=======
./src/xac97_l.o 

>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
C_DEPS += \
./src/aliens.d \
./src/bullet.d \
./src/bunker.d \
./src/end.d \
./src/main.d \
./src/pit.d \
./src/platform.d \
./src/saucer.d \
./src/sounds.d \
./src/stats.d \
./src/tank.d \
<<<<<<< HEAD
./src/xac97_l.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../hello_world_bsp_0/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


=======
./src/xac97_l.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../hello_world_bsp_0/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
