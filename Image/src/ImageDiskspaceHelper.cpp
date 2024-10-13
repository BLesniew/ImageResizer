#include "ImageDiskspaceHelper.hpp"

ImageDiskspaceHelper::ImageDiskspaceHelper(const cv::Mat &image)
{
    recalculate(image);
}

void ImageDiskspaceHelper::recalculate(const cv::Mat &image)
{
    encodeParams = {cv::IMWRITE_JPEG_QUALITY, jpgQuality, cv::IMWRITE_PNG_COMPRESSION, pngCompression};
    for (auto ext : allowedExtensions)
    {
        std::vector<u_char> buffer;
        cv::imencode(ext, image, buffer, encodeParams);
        mDiskspace[ext] = buffer.size();
    }
}

std::map<std::string, int> ImageDiskspaceHelper::getDiskspace()
{
    return mDiskspace;
}