################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/glad.c 

CPP_SRCS += \
../src/glfw-official-example.cpp \
../src/main.cpp 

OBJS += \
./src/glad.o \
./src/glfw-official-example.o \
./src/main.o 

C_DEPS += \
./src/glad.d 

CPP_DEPS += \
./src/glfw-official-example.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"C:\opengl-wrappers\glfw\include" -I"C:\opengl-wrappers\glfw-3.3.2\deps" -I"C:\opengl-wrappers\glad\include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"C:\opengl-wrappers\glad\include" -I"C:\opengl-wrappers\glfw-3.3.2\deps" -I"C:\opengl-wrappers\glfw\include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


