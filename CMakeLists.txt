cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -Wpedantic")

project(Resizer VERSION 1.0)

option(BUILD_TESTING "Build unit tests" ON)

# subdirectories

add_subdirectory(common)
add_subdirectory(Image)

if(BUILD_TESTING)
    enable_testing()
    find_package(Catch2 3 REQUIRED)
endif()

# executables

add_executable(Resizer main.cpp)
target_link_libraries(Resizer PUBLIC common Image)

if(BUILD_TESTING)
    add_subdirectory(common/testing)
    add_subdirectory(Image/testing)

endif()