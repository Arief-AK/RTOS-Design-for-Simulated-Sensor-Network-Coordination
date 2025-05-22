# Source files
file(GLOB_RECURSE SOURCES main.cpp "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Include directories
include_directories(include)
include_directories(src)

# Add the executable target
add_executable(RTOS_v1.0 ${SOURCES} ${HEADERS})

# Enable warnings conditionally based on the compiler
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(RTOS_v1.0 PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(RTOS_v1.0 PRIVATE /W4)
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
    src/CSVLogger.cpp
    src/JSONLogger.cpp
)
target_include_directories(scheduler_core_lib PUBLIC include)

# Link nlohmann_json to targets
target_link_libraries(RTOS_v1.0 PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(scheduler_core_lib PUBLIC nlohmann_json::nlohmann_json)

# Testing executables
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

add_executable(test_scheduler_cpu_idle tests/test_scheduler_cpu_idle.cpp)
target_link_libraries(test_scheduler_cpu_idle
    GTest::gtest_main
    scheduler_core_lib
)
add_test(NAME SchedulerCPUIdleTest COMMAND test_scheduler_cpu_idle)

add_executable(test_scheduler_resource_blocking tests/test_scheduler_resource_blocking.cpp)
target_link_libraries(test_scheduler_resource_blocking
    GTest::gtest_main
    scheduler_core_lib
)
add_test(NAME SchedulerResourceBlockingTest COMMAND test_scheduler_resource_blocking)