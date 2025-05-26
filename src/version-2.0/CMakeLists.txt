# Source files
file(GLOB_RECURSE SOURCES main.cpp "src/*.cpp" "src/*.tpp")
file(GLOB_RECURSE HEADERS "include/*.hpp" "config/*.hpp")

# Include directories
include_directories(config)
include_directories(include)
include_directories(src)

# Add the executable target
add_executable(RTOS_v2.0 ${SOURCES} ${HEADERS})

# Enable warnings conditionally based on the compiler
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(RTOS_v2.0 PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(RTOS_v2.0 PRIVATE /W4)
endif()