#include <catch2/catch_test_macros.hpp>
#include "UserInterface.hpp"

TEST_CASE("choiceFromInt maps integers to menu choices", "[UserInterface]")
{
    REQUIRE(choiceFromInt(1) == MenuChoice::ResizePx);
    REQUIRE(choiceFromInt(2) == MenuChoice::Crop);
    REQUIRE(choiceFromInt(3) == MenuChoice::ResizeFile);
    REQUIRE(choiceFromInt(4) == MenuChoice::Display);
    REQUIRE(choiceFromInt(5) == MenuChoice::Save);
    REQUIRE(choiceFromInt(6) == MenuChoice::Quit);
    REQUIRE(choiceFromInt(0) == MenuChoice::NoChoice);
    REQUIRE(choiceFromInt(7) == MenuChoice::NoChoice);
    REQUIRE(choiceFromInt(-1) == MenuChoice::NoChoice);
}
