#include "pch.h"
#include "ImageHistogram.h"
#include "ImageGrayscale.h"

typedef size_t (*ColorGetter)(ImagePixel);
std::vector<size_t> getHistogram(std::shared_ptr<Image> image, ColorGetter colorOf)
{
    std::vector<size_t> histogram(256, 0);
    for (size_t h = 0; h < image->Height; h++)
    {
        for (size_t w = 0; w < image->Width; w++)
        {
            //统计每种颜色的数量
            histogram[colorOf(image->Pixels[h * image->Width + w])]++;
        }
    }
    return histogram;
}

/// <summary>
/// 获取灰度化图像直方图数据
/// </summary>
/// <param name="image">图像</param>
/// <returns>直方图数据</returns>
std::vector<size_t> ImageHistogram::getHistogramGrayscale(std::shared_ptr<Image> image)
{
    return getHistogram(image, [](ImagePixel pixel) {
        ImagePixel grayscaled = ImageGrayscale::grayscale(pixel);
        return (size_t)grayscaled.R;
        });
}

/// <summary>
/// 获取图像红色通道直方图数据
/// </summary>
/// <param name="image">图像</param>
/// <returns>直方图数据</returns>
std::vector<size_t> ImageHistogram::getHistogramRed(std::shared_ptr<Image> image)
{
    return getHistogram(image, [](ImagePixel pixel) {
        return (size_t)pixel.R;
        });
}

/// <summary>
/// 获取图像绿色通道直方图数据
/// </summary>
/// <param name="image">图像</param>
/// <returns>直方图数据</returns>
std::vector<size_t> ImageHistogram::getHistogramGreen(std::shared_ptr<Image> image)
{
    return getHistogram(image, [](ImagePixel pixel) {
        return (size_t)pixel.G;
        });
}

/// <summary>
/// 获取图像蓝色通道直方图数据
/// </summary>
/// <param name="image">图像</param>
/// <returns>直方图数据</returns>
std::vector<size_t> ImageHistogram::getHistogramBlue(std::shared_ptr<Image> image)
{
    return getHistogram(image, [](ImagePixel pixel) {
        return (size_t)pixel.B;
        });
}
