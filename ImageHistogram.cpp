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
            //ͳ��ÿ����ɫ������
            histogram[colorOf(image->Pixels[h * image->Width + w])]++;
        }
    }
    return histogram;
}

/// <summary>
/// ��ȡ�ҶȻ�ͼ��ֱ��ͼ����
/// </summary>
/// <param name="image">ͼ��</param>
/// <returns>ֱ��ͼ����</returns>
std::vector<size_t> ImageHistogram::getHistogramGrayscale(std::shared_ptr<Image> image)
{
    return getHistogram(image, [](ImagePixel pixel) {
        ImagePixel grayscaled = ImageGrayscale::grayscale(pixel);
        return (size_t)grayscaled.R;
        });
}

/// <summary>
/// ��ȡͼ���ɫͨ��ֱ��ͼ����
/// </summary>
/// <param name="image">ͼ��</param>
/// <returns>ֱ��ͼ����</returns>
std::vector<size_t> ImageHistogram::getHistogramRed(std::shared_ptr<Image> image)
{
    return getHistogram(image, [](ImagePixel pixel) {
        return (size_t)pixel.R;
        });
}

/// <summary>
/// ��ȡͼ����ɫͨ��ֱ��ͼ����
/// </summary>
/// <param name="image">ͼ��</param>
/// <returns>ֱ��ͼ����</returns>
std::vector<size_t> ImageHistogram::getHistogramGreen(std::shared_ptr<Image> image)
{
    return getHistogram(image, [](ImagePixel pixel) {
        return (size_t)pixel.G;
        });
}

/// <summary>
/// ��ȡͼ����ɫͨ��ֱ��ͼ����
/// </summary>
/// <param name="image">ͼ��</param>
/// <returns>ֱ��ͼ����</returns>
std::vector<size_t> ImageHistogram::getHistogramBlue(std::shared_ptr<Image> image)
{
    return getHistogram(image, [](ImagePixel pixel) {
        return (size_t)pixel.B;
        });
}
