#pragma once

#include <opencv2/opencv.hpp>

#include "ImageBase.hpp"

class Image : public ImageBase
{
public:
    Image() = delete;
    Image(std::filesystem::path inputImagePath);

    bool save(std::filesystem::path path) const override;
    void display(std::string displayName = "Display") const override;
    void crop(ImgPoint corner1, ImgPoint corner2) override;
    ImgSize getSize() const override;
    void resizePx(ImgSize destinedSize) override;
    bool resizeFile(int destinedSize) override;

private:
    cv::Mat mImage;
    std::string mExtension;
};