cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -Wpedantic")

project(Resizer VERSION 1.0)
add_executable(Resizer src/main.cpp)

target_include_directories(Resizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)