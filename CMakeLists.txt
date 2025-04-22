cmake_minimum_required(VERSION 3.10.0)
project(RTOS_sensor_network VERSION 0.1.0 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB_RECURSE SOURCES main.cpp "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Include directories
include_directories(include)
include_directories(src)

# Add the executable target
add_executable(RTOS_sensor_network ${SOURCES} ${HEADERS})

# Enable warnings conditionally based on the compiler
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(RTOS_sensor_network PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(RTOS_sensor_network PRIVATE /W4)
endif()
