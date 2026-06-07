#pragma once

#include <filesystem>
#include <set>

namespace
{
    constexpr std::string_view JPG_EXT = ".jpg";
    constexpr std::string_view PNG_EXT = ".png";
    constexpr std::string_view TIFF_EXT = ".tiff";
}

class CliParser
{
public:
    // CliParser() = delete;
    CliParser(int argc, char *argv[]);

    std::filesystem::path inputFilePath, outputFilePath;

    bool isValid = true;
    bool isCompressionAllowed = true;
    std::set<std::string> allowedExtensions = {std::string{JPG_EXT}, std::string{PNG_EXT}, std::string{TIFF_EXT}};
};