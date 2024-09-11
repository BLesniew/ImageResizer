#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <catch2/catch_test_macros.hpp>

#include "common/include/UserInterface.hpp"

enum MenuChoice
{
    NoChoice = 0,
    ResizePx,
    CropResizePx,
    Crop,
    ResizeFile,
    Display,
    Quit
};

MenuChoice choiceFromInt(int choiceInt)
{
    switch (choiceInt)
    {
    case 1:
        return MenuChoice::ResizePx;

    case 2:
        return MenuChoice::CropResizePx;

    case 3:
        return MenuChoice::Crop;

    case 4:
        return MenuChoice::ResizeFile;

    case 5:
        return MenuChoice::Display;

    case 6:
        return MenuChoice::Quit;

    default:
        return MenuChoice::NoChoice;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        UI::printUsage();
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
            UI::printUsage();
            return -1;

        case -1:
            break;
        }

        break;
    }

    if (inputFilePath.empty())
    {
        UI::printUsage();
        return -1;
    }

    if (!std::filesystem::exists(inputFilePath))
    {
        std::cout << "File " << inputFilePath << " does not exist\n";
        return -1;
    }

    // std::cout << inputFilePath << std::endl;

    cv::Mat inputImage = cv::imread(inputFilePath);

    if (inputImage.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    ///////Main menu

    MenuChoice choice;

    do
    {
        UI::printMenu();

        int choiceInt;
        std::cin >> choiceInt;

        choice = choiceFromInt(choiceInt);

        if (choice == MenuChoice::Display)
        {
            std::cout << "Close the display to continue\n\n";

            std::string displayName = "Display";
            cv::namedWindow(displayName, cv::WINDOW_NORMAL);
            // cv::setWindowProperty(displayName, cv::WND_PROP_FULLSCREEN, cv::WINDOW_NORMAL);
            cv::imshow(displayName, inputImage);

            do
            {
                auto keyPressed = cv::waitKey(100);

                if (keyPressed == 27) // Esc button
                {
                    cv::destroyWindow(displayName);
                    break;
                }

            } while (cv::getWindowProperty(displayName, cv::WND_PROP_VISIBLE) > 0);
        }
    } while (choice == MenuChoice::Display || choice == MenuChoice::NoChoice);

    if (choice == MenuChoice::Quit)
    {
        return 0;
    }
}