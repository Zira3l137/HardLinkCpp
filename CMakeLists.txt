cmake_minimum_required(VERSION 3.24)
project(HardLinkCpp CXX VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

set(SOURCES
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/linker.cpp"
    "${CMAKE_SOURCE_DIR}/src/logger.cpp"
    "${CMAKE_SOURCE_DIR}/src/utils.cpp"
    "${CMAKE_SOURCE_DIR}/src/argparser.cpp"
)

add_executable(HardLinkCpp ${SOURCES})
target_include_directories(HardLinkCpp PUBLIC "${CMAKE_SOURCE_DIR}/include")

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(HardLinkCpp PUBLIC
        -Wall
        -Wextra
        -Wpedantic
        -O3 # Optimization flag
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(HardLinkCpp PUBLIC
        /W4 # Warning level 4
        /O2 # Optimization flag
    )
endif()
