add_executable(common_tests test_choice.cpp test_cli_parser.cpp)
target_link_libraries(common_tests PRIVATE common Catch2::Catch2WithMain)
add_test(NAME common_tests COMMAND common_tests)
