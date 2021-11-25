#include "pch.h"
#include <numeric>
#include "ImageEqualize.h"
#include "ImageHistogram.h"

/// <summary>
/// ���⻯ͼ��ֱ��ͼ
/// </summary>
/// <param name="image">ԭͼ</param>
/// <returns>ֱ��ͼ���⻯ͼ��</returns>
std::shared_ptr<Image> ImageEqualize::equalize(std::shared_ptr<Image> image)
{
    //ֱ��ͼ���⻯ǰ��ͼƬ��С����
    auto equalized = std::make_shared<Image>(*image);

    //RGB��ɫֱ��ͼ����
    auto histogramRed = ImageHistogram::getHistogramRed(image);
    auto histogramGreen = ImageHistogram::getHistogramGreen(image);
    auto histogramBlue = ImageHistogram::getHistogramBlue(image);

    //ͼ�����
    size_t area = image->Height * image->Width;

    //RGB��ɫ���⻯ӳ���
    std::vector<BYTE> equalizedRed(256);
    std::vector<BYTE> equalizedGreen(256);
    std::vector<BYTE> equalizedBlue(256);

    //ʹ���ۻ��ֲ�������CDF�����������ɫ��ӳ��
    for (size_t i = 0; i < 256; i++)
    {
        equalizedRed[i] = (BYTE)(std::accumulate(histogramRed.begin(), histogramRed.begin() + i, (uint64_t)0) * 255 / area);
        equalizedGreen[i] = (BYTE)(std::accumulate(histogramGreen.begin(), histogramGreen.begin() + i, (uint64_t)0) * 255 / area);
        equalizedBlue[i] = (BYTE)(std::accumulate(histogramBlue.begin(), histogramBlue.begin() + i, (uint64_t)0) * 255 / area);
    }

    //��ӳ��ͼ������
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
