#include <catch2/catch_test_macros.hpp>
#include <filesystem>

#include "Image.hpp"

SCENARIO("Image can be loaded, resized, and saved", "[Image]")
{
    const std::filesystem::path sampleImagePath(TEST_IMAGE_PATH);
    REQUIRE(std::filesystem::exists(sampleImagePath));

    GIVEN("a loaded image from the sample file")
    {
        Image image(sampleImagePath);
        auto originalSize = image.getSize();
        REQUIRE(originalSize.width > 0);
        REQUIRE(originalSize.height > 0);

        WHEN("the image is resized by pixel dimensions")
        {
            const ImgSize newSize{originalSize.width / 2, originalSize.height / 2};
            image.resizePx(newSize);

            THEN("the image dimensions are updated")
            {
                auto resizedSize = image.getSize();
                REQUIRE(resizedSize.width == newSize.width);
                REQUIRE(resizedSize.height == newSize.height);
            }
        }

        WHEN("the image is saved to a temporary file")
        {
            const auto outPath = std::filesystem::temp_directory_path() / "resizer_test_output";
            const auto result = image.save(outPath);

            THEN("the save operation succeeds and the output file exists")
            {
                REQUIRE(result == true);
                REQUIRE(std::filesystem::exists(outPath.string() + ".jpg"));
                std::filesystem::remove(outPath.string() + ".jpg");
            }
        }
    }
}
