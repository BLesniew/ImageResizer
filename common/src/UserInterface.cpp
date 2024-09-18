#include "UserInterface.hpp"

void UI::printUsage()
{
    std::cout << "Usage: ./Resizer <-i input_file_path> [-o output_file_path]\n";
}

void UI::printMenu()
{
    std::cout << "=============== WHAT DO YOU WANT TO DO? ===============\n";
    std::cout << "1) Resize the image to pixel size\n";
    std::cout << "2) Crop the image\n";
    std::cout << "3) Shrink the file size (in MB)\n";
    std::cout << "4) Display the image\n";
    std::cout << "5) Save the image\n";
    std::cout << "6) Exit the app\n";
}

MenuChoice UI::getMenuChoice()
{
    int choiceInt;
    std::cin >> choiceInt;
    return choiceFromInt(choiceInt);
}

void UI::printWrongInput()
{
    std::cerr << "ERROR: Wrong data entered, change not applied\n\n";
}

void UI::printNothingToSave()
{
    std::cout << "Nothing changed in the image - save aborted\n\n";
}

void UI::printSaveFailed()
{
    std::cout << "File save failed, path forgotten, please try again\n";
}

std::pair<ImgPoint, ImgPoint> UI::getCropCorners()
{
    ImgPoint corner1, corner2;

    std::cout << "A picture between two given points will remain\n";
    std::cout << "First point:\n";
    std::cout << "x = ";
    std::cin >> corner1.x;
    std::cout << "y = ";
    std::cin >> corner1.y;
    std::cout << "Second point:\n";
    std::cout << "x = ";
    std::cin >> corner2.x;
    std::cout << "y = ";
    std::cin >> corner2.y;
    std::cout << std::endl;

    return {corner1, corner2};
}

std::filesystem::path UI::getOutputPath()
{
    std::string path;
    std::cout << "Enter the path where the image will be saved (without an extension):\n";
    std::cin >> path;

    return {path};
}

bool UI::getQuitUnsaved()
{
    char choice;
    std::cout << "Changes made but not saved to any file, quit anyways? (y/n)\n";
    std::cin >> choice;

    return choice == 'y';
}

ImgSize UI::getDestinedSize()
{
    ImgSize destinedSize;
    std::cout << "Enter destined image size:\n";
    std::cout << "width = ";
    std::cin >> destinedSize.width;
    std::cout << "height = ";
    std::cin >> destinedSize.height;

    return destinedSize;
}

MenuChoice choiceFromInt(int choiceInt)
{
    switch (choiceInt)
    {
    case 1:
        return MenuChoice::ResizePx;

    case 2:
        return MenuChoice::Crop;

    case 3:
        return MenuChoice::ResizeFile;

    case 4:
        return MenuChoice::Display;

    case 5:
        return MenuChoice::Save;

    case 6:
        return MenuChoice::Quit;

    default:
        return MenuChoice::NoChoice;
    }
}
