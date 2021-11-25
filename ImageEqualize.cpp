#include "pch.h"
#include <numeric>
#include "ImageEqualize.h"
#include "ImageHistogram.h"

/// <summary>
/// 均衡化图像直方图
/// </summary>
/// <param name="image">原图</param>
/// <returns>直方图均衡化图像</returns>
std::shared_ptr<Image> ImageEqualize::equalize(std::shared_ptr<Image> image)
{
    //直方图均衡化前后图片大小不变
    auto equalized = std::make_shared<Image>(*image);

    //RGB各色直方图数据
    auto histogramRed = ImageHistogram::getHistogramRed(image);
    auto histogramGreen = ImageHistogram::getHistogramGreen(image);
    auto histogramBlue = ImageHistogram::getHistogramBlue(image);

    //图像面积
    size_t area = image->Height * image->Width;

    //RGB各色均衡化映射表
    std::vector<BYTE> equalizedRed(256);
    std::vector<BYTE> equalizedGreen(256);
    std::vector<BYTE> equalizedBlue(256);

    //使用累积分布函数（CDF）计算各个颜色的映射
    for (size_t i = 0; i < 256; i++)
    {
        equalizedRed[i] = (BYTE)(std::accumulate(histogramRed.begin(), histogramRed.begin() + i, (uint64_t)0) * 255 / area);
        equalizedGreen[i] = (BYTE)(std::accumulate(histogramGreen.begin(), histogramGreen.begin() + i, (uint64_t)0) * 255 / area);
        equalizedBlue[i] = (BYTE)(std::accumulate(histogramBlue.begin(), histogramBlue.begin() + i, (uint64_t)0) * 255 / area);
    }

    //重映射图像像素
    for (size_t h = 0; h < image->Height; h++)
    {
        for (size_t w = 0; w < image->Width; w++)
        {
            auto pixel = image->Pixels[h * image->Width + w];
            equalized->Pixels[h * image->Width + w] = ImagePixel(pixel.A, equalizedRed[pixel.R], equalizedGreen[pixel.G], equalizedBlue[pixel.B]);
        }
    }

    return equalized;
}
