#pragma once

#include <opencv2/opencv.hpp>

#include <map>
#include <string>

// This class will take an image and calculate output file size when certain extension is used
// (for all extensions in allowedExtensions list)
class ImageDiskspaceHelper
{
public:
    ImageDiskspaceHelper() = delete;
    ImageDiskspaceHelper(const cv::Mat &image);

    void recalculate(const cv::Mat &image);
    std::map<std::string, int> getDiskspace();

    // TODO: getters and setters
    inline static std::set<std::string> allowedExtensions = {".jpg", ".png", ".tiff"};
    inline static int jpgQuality = 100;
    inline static int pngCompression = 9;
    inline static std::vector<int> encodeParams = {cv::IMWRITE_JPEG_QUALITY, jpgQuality, cv::IMWRITE_PNG_COMPRESSION, pngCompression};
    // TODO: optimize tiff compression

private:
    std::map<std::string, int> mDiskspace;
};