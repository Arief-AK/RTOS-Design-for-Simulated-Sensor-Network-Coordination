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
add_executable(v2-test_scheduler_sanity tests/test_scheduler_sanity.cpp)
target_link_libraries(v2-test_scheduler_sanity
    GTest::gtest_main
    rtos_core
)
add_test(NAME V2_SchedulerSanityTest COMMAND v2-test_scheduler_sanity)

add_executable(v2-test_preemptive_kernel tests/test_preemptive_kernel.cpp)
target_link_libraries(v2-test_preemptive_kernel
    GTest::gtest_main
    rtos_core
)
add_test(NAME V2_PreemptiveKernelTest COMMAND v2-test_preemptive_kernel)

add_executable(v2-test_deadline_miss tests/test_deadline_miss.cpp)
target_link_libraries(v2-test_deadline_miss
    GTest::gtest_main
    rtos_core
)
add_test(NAME V2_DeadlineMissTest COMMAND v2-test_deadline_miss)

add_executable(v2-test_preemption_count tests/test_preemption_count.cpp)
target_link_libraries(v2-test_preemption_count
    GTest::gtest_main
    rtos_core
)
add_test(NAME V2_PreemptionCountTest COMMAND v2-test_preemption_count)

add_executable(v2-test_stress tests/test_stress.cpp)
target_link_libraries(v2-test_stress
    GTest::gtest_main
    rtos_core
)
add_test(NAME V2_StressTest COMMAND v2-test_stress)