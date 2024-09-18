#pragma once

#include <iostream>
#include <filesystem>

#include "ImageTypes.hpp"

enum MenuChoice
{
    NoChoice = 0,
    ResizePx,
    Crop,
    ResizeFile,
    Display,
    Save,
    Quit
};

class UI
{
public:
    static void printUsage();
    static void printMenu();
    static void printWrongInput();
    static void printNothingToSave();
    static void printSaveFailed();

    static std::pair<ImgPoint, ImgPoint> getCropCorners();
    static std::filesystem::path getOutputPath();
    static bool getQuitUnsaved();
    static ImgSize getDestinedSize();

    template <typename T>
    static void printSize(T size)
    {
        std::cout << "Current image size:\n";
        std::cout << "width = " << size.width << "px, height = " << size.height << "px\n";
    }

    static MenuChoice getMenuChoice();
};

MenuChoice choiceFromInt(int choiceInt);