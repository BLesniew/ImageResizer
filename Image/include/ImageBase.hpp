#pragma once

#include <filesystem>
#include "ImageTypes.hpp"

class ImageBase
{
public:
    virtual bool save(std::filesystem::path path) const = 0;
    virtual void display(std::string displayName = "Display") const = 0;
    virtual void crop(ImgPoint corner1, ImgPoint corner2) = 0;
    virtual ImgSize getSize() const = 0;
    virtual void resizePx(ImgSize destinedSize) = 0;
    virtual bool resizeFile(int destinedSize) = 0;
};