cmake_minimum_required(VERSION 3.1)

project(Extra_Snake)
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE PROJECT_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(snake test/test.cc ${PROJECT_SOURCES})
target_link_libraries(snake raylib)