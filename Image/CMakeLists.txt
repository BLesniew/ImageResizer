add_library(Image STATIC src/Image.cpp)
target_include_directories(Image PUBLIC include)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})