#include "Image.hpp"

#include "ImageDiskspaceHelper.hpp"

Image::Image(std::filesystem::path inputFilePath)
{
    auto extension = inputFilePath.extension();

    if (ImageDiskspaceHelper::allowedExtensions.find(extension) != ImageDiskspaceHelper::allowedExtensions.end())
    {
        mExtension = extension;
    }
    else
    {
        mExtension = *ImageDiskspaceHelper::allowedExtensions.begin();
    }

    if (!std::filesystem::exists(inputFilePath))
    {
        throw std::runtime_error("File " + inputFilePath.string() + " does not exist");
    }

    mImage = cv::imread(inputFilePath);

    if (mImage.empty())
    {
        throw std::runtime_error("Could not open or find the image under given path: " + inputFilePath.string());
    }
}

void Image::display(std::string displayName) const
{
    cv::namedWindow(displayName, cv::WINDOW_NORMAL);
    // cv::setWindowProperty(displayName, cv::WND_PROP_FULLSCREEN, cv::WINDOW_NORMAL);
    cv::imshow(displayName, mImage);

    do
    {
        auto keyPressed = cv::waitKey(100);

        if (keyPressed == 27) // Esc button
        {
            cv::destroyWindow(displayName);
            break;
        }

    } while (cv::getWindowProperty(displayName, cv::WND_PROP_VISIBLE) > 0);
}

bool Image::save(std::filesystem::path path) const
{
    return cv::imwrite(path.string() + mExtension, mImage, ImageDiskspaceHelper::encodeParams);
}

// right collumn and bottom row not included
void Image::crop(ImgPoint corner1, ImgPoint corner2)
{
    int xMin = std::min(corner1.x, corner2.x);
    int yMin = std::min(corner1.y, corner2.y);
    int width = std::abs(corner1.x - corner2.x);
    int height = std::abs(corner1.y - corner2.y);

    mImage = mImage(cv::Rect(xMin, yMin, width, height));
}

ImgSize Image::getSize() const
{
    return {mImage.size().width, mImage.size().height};
}

void Image::resizePx(ImgSize destinedSize)
{
    cv::resize(mImage, mImage, {destinedSize.width, destinedSize.height});
}

// TODO: check if can be optimized
// TODO: add extension choice and isCompressionAllowed flag
bool Image::resizeFile(int destinedSize)
{
    ImageDiskspaceHelper diskspaceHelper(mImage);

    const std::string initialExtension = mExtension;
    bool isResized = false;

    auto diskpaceMap = diskspaceHelper.getDiskspace();
    auto bestExtension = std::min_element(diskpaceMap.begin(), diskpaceMap.end(),
                                          [](const auto &l, const auto &r)
                                          { return l.second < r.second; });
    mExtension = bestExtension->first;

    int minJpgQuality = 80;
    while (bestExtension->second > destinedSize && ImageDiskspaceHelper::jpgQuality > minJpgQuality)
    {
        int qualityLoweringStep = 2;
        if (ImageDiskspaceHelper::jpgQuality > minJpgQuality)
        {
            ImageDiskspaceHelper::jpgQuality = std::max(ImageDiskspaceHelper::jpgQuality - qualityLoweringStep, minJpgQuality);
        }

        diskspaceHelper.recalculate(mImage);
        diskpaceMap = diskspaceHelper.getDiskspace();
        bestExtension = std::min_element(diskpaceMap.begin(), diskpaceMap.end(),
                                         [](const auto &l, const auto &r)
                                         { return l.second < r.second; });
        mExtension = bestExtension->first;
    }

    while (bestExtension->second > destinedSize)
    {
        int imageResizePxPercentStep = 10;

        ImgSize newSize;
        newSize.width = (int)(mImage.size().width * 0.01 * (100 - imageResizePxPercentStep));
        newSize.height = (int)(mImage.size().height * 0.01 * (100 - imageResizePxPercentStep));

        resizePx(newSize);
        isResized = true;

        diskspaceHelper.recalculate(mImage);
        diskpaceMap = diskspaceHelper.getDiskspace();
        bestExtension = std::min_element(diskpaceMap.begin(), diskpaceMap.end(),
                                         [](const auto &l, const auto &r)
                                         { return l.second < r.second; });
        mExtension = bestExtension->first;
    }

    return isResized || (initialExtension != mExtension);
}
