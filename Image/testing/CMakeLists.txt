add_executable(image_tests test_image.cpp)
target_link_libraries(image_tests PRIVATE Image Catch2::Catch2WithMain)
target_compile_definitions(image_tests PRIVATE TEST_IMAGE_PATH="${CMAKE_SOURCE_DIR}/lena-alone.jpg")
add_test(NAME image_tests COMMAND image_tests)
