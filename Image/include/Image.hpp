#pragma once

#include <filesystem>
#include <opencv2/opencv.hpp>

class Image
{
public:
    Image() = delete;
    Image(std::filesystem::path inputImagePath);

    bool save(std::filesystem::path path);
    void display();

private:
    cv::Mat mImage;
};