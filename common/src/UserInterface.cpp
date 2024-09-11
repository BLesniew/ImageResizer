#include "UserInterface.hpp"

void UI::printUsage()
{
    std::cout << "Usage: ./Resizer <-i input_file_path> [-o output_file_path]\n";
}

void UI::printMenu()
{
    std::cout << "=============== WHAT DO YOU WANT TO DO? ===============\n";
    std::cout << "1) Resize the image to pixel size\n";
    std::cout << "2) Crop the image then resize to pixel size\n";
    std::cout << "3) Crop the image\n";
    std::cout << "4) Shrink the file size (in MB)\n";
    std::cout << "5) Display the image\n";
    std::cout << "6) Exit the app\n";
}