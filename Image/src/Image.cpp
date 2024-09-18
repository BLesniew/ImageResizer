#include "Image.hpp"

Image::Image(std::filesystem::path inputFilePath)
{
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
    return cv::imwrite(path.string() + mExtension, mImage);
}

void Image::crop(ImgPoint corner1, ImgPoint corner2)
{
    // TODO: Check if inclusive

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

void Image::resize(ImgSize destinedSize)
{
    cv::resize(mImage, mImage, {destinedSize.width, destinedSize.height});
}