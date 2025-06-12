# Source files
file(GLOB_RECURSE SOURCES main.cpp "src/*.cpp" "src/*.tpp" "simulation/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp" "config/*.hpp" "simulation/*.hpp")

# Include directories
include_directories(config)
include_directories(include)
include_directories(src)
include_directories(simulation)

# Create a core library for RTOS logic (exclude main.cpp)
set(RTOS_CORE_SOURCES)
foreach(src ${SOURCES})
    if(NOT src MATCHES "main.cpp")
        list(APPEND RTOS_CORE_SOURCES ${src})
    endif()
endforeach()

add_library(rtos_core ${RTOS_CORE_SOURCES} ${HEADERS})
target_link_libraries(rtos_core PUBLIC nlohmann_json::nlohmann_json)

# Main executable links to the core library
add_executable(RTOS_v2.0 main.cpp)
target_link_libraries(RTOS_v2.0 PRIVATE rtos_core)

# Enable warnings conditionally based on the compiler
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(RTOS_v2.0 PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(RTOS_v2.0 PRIVATE /W4)
endif()

# Link nlohmann_json to targets
target_link_libraries(RTOS_v2.0 PRIVATE nlohmann_json::nlohmann_json)

# Testing executables
add_executable(v2-test_kernel_features tests/test_kernel_features.cpp)
target_link_libraries(v2-test_kernel_features
    GTest::gtest_main
    rtos_core
)
add_test(NAME V2_KernelFeaturesTest COMMAND v2-test_kernel_features)

add_executable(v2-test_rm_scheduler tests/test_rm_scheduler.cpp)
target_link_libraries(v2-test_rm_scheduler
    GTest::gtest_main
    rtos_core
)
add_test(NAME V2_RMSchedulerTest COMMAND v2-test_rm_scheduler)

add_executable(v2-test_priority_scheduler tests/test_priority_scheduler.cpp)
target_link_libraries(v2-test_priority_scheduler
    GTest::gtest_main
    rtos_core
)
add_test(NAME V2_PrioritySchedulerTest COMMAND v2-test_priority_scheduler)