#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <catch2/catch_test_macros.hpp>

#include "common/include/UserInterface.hpp"
#include "Image/include/Image.hpp"

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

    Image image(inputFilePath);

    ///////Main menu

    MenuChoice choice;
    bool imageChanged = false;
    bool imageSaved = false;

    do
    {
        UI::printMenu();

        choice = UI::getMenuChoice();

        switch (choice)
        {
        case MenuChoice::ResizePx:
        {
            auto destinedSize = UI::getDestinedSize();

            try
            {
                image.resize(destinedSize);
                imageChanged = true;
            }
            catch (const std::exception &e)
            {
                UI::printWrongInput();
            }
        }
        break;

        case MenuChoice::Crop:
        {
            // TODO: pick crop corners on displayed image
            UI::printSize(image.getSize());

            auto corners = UI::getCropCorners();

            try
            {
                image.crop(corners.first, corners.second);
                imageChanged = true;
            }
            catch (const std::exception &e)
            {
                UI::printWrongInput();
            }
        }
        break;

        case MenuChoice::ResizeFile:
            break;

        case MenuChoice::Display:
            std::cout << "Close the display to continue\n\n";
            image.display();
            break;

        case MenuChoice::Save:
            if (!imageChanged)
            {
                UI::printNothingToSave();
                break;
            }

            if (outputFilePath.empty())
            {
                outputFilePath = UI::getOutputPath();
            }

            if (image.save(outputFilePath))
            {
                imageSaved = true;
            }
            else
            {
                UI::printSaveFailed();
                outputFilePath.clear();
            }
            break;

        case MenuChoice::Quit:
            if (imageChanged && !imageSaved)
            {
                if (UI::getQuitUnsaved())
                {
                    return 0;
                }

                break;
            }
            return 0;

        case MenuChoice::NoChoice:
            std::cout << "Wrong choice value\n\n";
            break;
        }
    } while (true);
}