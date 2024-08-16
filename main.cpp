#include <iostream>
#include <filesystem>
#include <unistd.h>
#include "Module1/include/testing.h"
#include <opencv2/opencv.hpp>
#include <catch2/catch_test_macros.hpp>

enum MenuChoice
{
    ResizePx,
    CropResizePx,
    Crop,
    ResizeFile,
    Display,
    Quit
};

void displayUsage()
{
    std::cout << "Usage: ./Resizer <-i input_file_path> [-o output_file_path]\n";
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        displayUsage();
        return -1;
    }

    std::filesystem::path inputFilePath, outputFilePath;

    for (;;)
    {
        switch (getopt(argc, argv, "i:o:"))
        {
        case 'i':
            inputFilePath = optarg;
            continue;

        case 'o':
            outputFilePath = optarg;
            continue;

        default:
            displayUsage();
            return -1;

        case -1:
            break;
        }

        break;
    }

    if (inputFilePath.empty())
    {
        displayUsage();
        return -1;
    }

    if (!std::filesystem::exists(inputFilePath))
    {
        std::cout << "File " << inputFilePath << " does not exist\n";
    }

    // std::cout << inputFilePath << std::endl;

    cv::Mat inputImage = cv::imread(inputFilePath);

    if (inputImage.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    ////////////////////////////////START

    cv::namedWindow("test");

    cv::imshow("test", inputImage);

    cv::waitKey(0);

    ////////////////////////////////END

    return 0;
}