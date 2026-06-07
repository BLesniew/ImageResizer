#include <catch2/catch_test_macros.hpp>
#include <unistd.h>
#include "CliParser.hpp"

SCENARIO("CLI parser validates and stores arguments", "[CliParser]")
{
    GIVEN("command line arguments with input and output paths")
    {
        const char *argv[] = {"resizer", "-i", "test.jpg", "-o", "output.jpg"};
        int argc = 5;

        WHEN("CliParser is initialized")
        {
            optind = 1;
            CliParser parser(argc, const_cast<char **>(argv));

            THEN("parser is valid and paths are set correctly")
            {
                REQUIRE(parser.isValid == true);
                REQUIRE(parser.inputFilePath == "test.jpg");
                REQUIRE(parser.outputFilePath == "output.jpg");
            }
        }
    }

    GIVEN("command line arguments with only input path")
    {
        const char *argv[] = {"resizer", "-i", "test.jpg"};
        int argc = 3;

        WHEN("CliParser is initialized")
        {
            optind = 1;
            CliParser parser(argc, const_cast<char **>(argv));

            THEN("parser is valid and output path is empty")
            {
                REQUIRE(parser.isValid == true);
                REQUIRE(parser.inputFilePath == "test.jpg");
                REQUIRE(parser.outputFilePath.empty());
            }
        }
    }

    GIVEN("insufficient command line arguments")
    {
        const char *argv[] = {"resizer"};
        int argc = 1;

        WHEN("CliParser is initialized")
        {
            optind = 1;
            CliParser parser(argc, const_cast<char **>(argv));

            THEN("parser is invalid")
            {
                REQUIRE(parser.isValid == false);
            }
        }
    }
}
