#include "pch.h"
#include "ImageGrayscale.h"

/// <summary>
/// ��ÿ�����ص�RGBֵ��ƽ����Ϊ��Ӧ���صĻҶ�ֵ
/// </summary>
/// <param name="image">ԭͼ</param>
/// <returns>�ҶȻ�ͼƬ</returns>
std::shared_ptr<Image> ImageGrayscale::grayscale(std::shared_ptr<Image> image)
{
	//�ҶȻ�ǰ��ͼƬ��С����
	auto grayscaled = std::make_shared<Image>(image->Width, image->Height);

	for (size_t h = 0; h < grayscaled->Height; h++)
	{
		for (size_t w = 0; w < grayscaled->Width; w++)
		{
			auto pixel = image->Pixels[h * image->Width + w];
			grayscaled->Pixels[h * image->Width + w] = grayscale(pixel);
		}
	}

	return grayscaled;
}

/// <summary>
/// �����ص�RGBֵ��ƽ����Ϊ��Ҷ�ֵ
/// </summary>
/// <param name="pixel">ԭ����</param>
/// <returns>�ҶȻ�����</returns>
ImagePixel ImageGrayscale::grayscale(ImagePixel pixel)
{
	//��RGBֵ��ƽ����Ϊ�Ҷ�ֵ
	BYTE grayValue = (pixel.R + pixel.G + pixel.B) / 3;
	return ImagePixel(pixel.A, grayValue, grayValue, grayValue);
}
