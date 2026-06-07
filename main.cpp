#include <iostream>
#include <filesystem>
#include <unistd.h>

#include "common/include/UserInterface.hpp"
#include "common/include/CliParser.hpp"
#include "Image/include/Image.hpp"

int main(int argc, char *argv[])
{
    CliParser parser(argc, argv);
    if(!parser.isValid) {
        UI::printUsage();
        return -1;
    }

    if (!std::filesystem::exists(parser.inputFilePath) || !std::filesystem::is_regular_file(parser.inputFilePath))
    {
        UI::printInputFileNotFound(parser.inputFilePath);
        UI::printUsage();
        return -1;
    }

    Image image(parser.inputFilePath);

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
            auto destinedSize = UI::getDestinedSizePx();

            try
            {
                image.resizePx(destinedSize);
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
        {
            auto destinedSize = UI::getDestinedFileSize();

            if (image.resizeFile(destinedSize))
            {
                imageChanged = true;
            }
        }
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

            if (parser.outputFilePath.empty())
            {
                parser.outputFilePath = UI::getOutputPath();
            }

            if (image.save(parser.outputFilePath))
            {
                imageSaved = true;
            }
            else
            {
                UI::printSaveFailed();
                parser.outputFilePath.clear();
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