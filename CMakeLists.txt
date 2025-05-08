cmake_minimum_required(VERSION 3.10.0)
project(RTOS_sensor_network VERSION 0.1.0 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Add GoogleTest framework
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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

# Library definitions
add_library(scheduler_core_lib
    src/TaskController.cpp
    src/RoundRobinScheduler.cpp
    src/Resource.cpp
    src/SimulationEngine.cpp
    src/PriorityScheduler.cpp
    src/ConsoleLogger.cpp
    src/MetricsCollector.cpp
    src/EDFScheduler.cpp
)
target_include_directories(scheduler_core_lib PUBLIC include)

# Testing executables
add_executable(test_round_robin_scheduler tests/test_round_robin.cpp)
target_link_libraries(test_round_robin_scheduler
    GTest::gtest_main
    scheduler_core_lib
)
add_test(NAME RoundRobinSchedulerTest COMMAND test_round_robin_scheduler)

add_executable(test_priority_scheduler tests/test_priority.cpp)
target_link_libraries(test_priority_scheduler
    GTest::gtest_main
    scheduler_core_lib
)
add_test(NAME PrioritySchedulerTest COMMAND test_priority_scheduler)

add_executable(test_edf_scheduler tests/test_edf.cpp)
target_link_libraries(test_edf_scheduler
    GTest::gtest_main
    scheduler_core_lib
)
add_test(NAME EDFSchedulerTest COMMAND test_edf_scheduler)

add_executable(test_scheduler_sanity tests/test_scheduler_sanity.cpp)
target_link_libraries(test_scheduler_sanity
    GTest::gtest_main
    scheduler_core_lib
)
add_test(NAME SchedulerSanityTest COMMAND test_scheduler_sanity)

add_executable(test_scheduler_context_switch tests/test_scheduler_context_switch.cpp)
target_link_libraries(test_scheduler_context_switch
    GTest::gtest_main
    scheduler_core_lib
)
add_test(NAME SchedulerContextSwitchTest COMMAND test_scheduler_context_switch)

add_executable(test_scheduler_response_time tests/test_scheduler_response_time.cpp)
target_link_libraries(test_scheduler_response_time
    GTest::gtest_main
    scheduler_core_lib
)
add_test(NAME SchedulerResponseTimeTest COMMAND test_scheduler_response_time)