cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -Wpedantic")

project(Resizer VERSION 1.0)

# subdirectories

add_subdirectory(Module1)

# libraries

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Catch2 3 REQUIRED)

# executables

add_executable(Resizer main.cpp)
target_link_libraries(Resizer PUBLIC Module1 ${OpenCV_LIBS} Catch2::Catch2WithMain)