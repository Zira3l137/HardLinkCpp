cmake_minimum_required(VERSION 3.24.0)

project(
  HardLinkCpp
  VERSION 1.1
  DESCRIPTION "HardLinkCpp"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    "${CMAKE_SOURCE_DIR}/src/main.cpp" "${CMAKE_SOURCE_DIR}/src/linker.cpp"
    "${CMAKE_SOURCE_DIR}/src/logger.cpp" "${CMAKE_SOURCE_DIR}/src/utils.cpp"
    "${CMAKE_SOURCE_DIR}/src/argparser.cpp")

add_executable(HardLinkCpp ${SOURCES})
target_include_directories(HardLinkCpp PUBLIC "${CMAKE_SOURCE_DIR}/include")
