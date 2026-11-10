cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME Homework5)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        Bsp/bsp_dwt.c
        Bsp/bsp_PWM.c

        Components/Algorithm/QuaternionEKF.c
        Components/Controller/controller.c
        Components/Devices/BMI088driver.c
        Components/Devices/BMI088Middleware.c
        Components/Devices/transfer_function.c
        Components/user_lib.c
        Components/kalman_filter.c

        User_Driver/pid_driver.cpp
        User_Driver/Motor_Driver/DJI_motor_driver.cpp
        User_Driver/Motor_Driver/motor_driver.cpp

        Application/ins_task.c
        Application/Power_Check_Task.cpp
        Application/Power_Check_Task.h
        Application/DBUS_Task.h
        Application/DBUS_Task.cpp
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
        Bsp/CAN_bsp.cpp
        Application/Motor_Task.cpp
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
        Bsp/CAN_bsp.h
        Application/Motor_Task.h
        ${PROJECT_SOURCE_DIR}/User_Driver
        ${PROJECT_SOURCE_DIR}/User_Driver/Motor_Driver
        ${PROJECT_SOURCE_DIR}/Bsp
        ${PROJECT_SOURCE_DIR}/Application
        ${PROJECT_SOURCE_DIR}/Components
        ${PROJECT_SOURCE_DIR}/Components/Devices
        ${PROJECT_SOURCE_DIR}/Components/Algorithm
        ${PROJECT_SOURCE_DIR}/Components/Controller
        ${PROJECT_SOURCE_DIR}/Components/Include
        ${PROJECT_SOURCE_DIR}/Components/PrivateInclude
        ${PROJECT_SOURCE_DIR}/Components/Source
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)


set(DSP_SOURCES
        # BasicMathFunctions 基础数学
        ${PROJECT_SOURCE_DIR}/Components/Source/BasicMathFunctions/arm_add_f32.c
        ${PROJECT_SOURCE_DIR}/Components/Source/BasicMathFunctions/arm_sub_f32.c
        ${PROJECT_SOURCE_DIR}/Components/Source/BasicMathFunctions/arm_mult_f32.c

        # MatrixFunctions 矩阵（EKF卡尔曼核心）
        ${PROJECT_SOURCE_DIR}/Components/Source/MatrixFunctions/arm_mat_init_f32.c
        ${PROJECT_SOURCE_DIR}/Components/Source/MatrixFunctions/arm_mat_add_f32.c
        ${PROJECT_SOURCE_DIR}/Components/Source/MatrixFunctions/arm_mat_sub_f32.c
        ${PROJECT_SOURCE_DIR}/Components/Source/MatrixFunctions/arm_mat_mult_f32.c
        ${PROJECT_SOURCE_DIR}/Components/Source/MatrixFunctions/arm_mat_trans_f32.c
        ${PROJECT_SOURCE_DIR}/Components/Source/MatrixFunctions/arm_mat_inverse_f32.c

        # SupportFunctions 三角函数 arm_sin_f32 / arm_cos_f32
        ${PROJECT_SOURCE_DIR}/Components/Source/FastMathFunctions/arm_sin_f32.c
        ${PROJECT_SOURCE_DIR}/Components/Source/FastMathFunctions/arm_cos_f32.c
        ${PROJECT_SOURCE_DIR}/Components/Source/CommonTables/CommonTables.c
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${DSP_SOURCES})


# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
