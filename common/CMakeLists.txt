add_library(UserInterface STATIC src/UserInterface.cpp)
target_include_directories(UserInterface PUBLIC include)

target_link_libraries(UserInterface PUBLIC Image)