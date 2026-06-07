add_library(CliParser STATIC src/CliParser.cpp)
target_include_directories(CliParser PUBLIC include)

add_library(UserInterface STATIC src/UserInterface.cpp)
target_include_directories(UserInterface PUBLIC include)
target_link_libraries(UserInterface PUBLIC Image)

# Umbrella library grouping common components
add_library(common INTERFACE)
target_link_libraries(common INTERFACE CliParser UserInterface)