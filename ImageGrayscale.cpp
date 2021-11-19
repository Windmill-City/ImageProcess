#include "pch.h"
#include "ImageGrayscale.h"

/// <summary>
/// 对每个像素的RGB值求平均即为对应像素的灰度值
/// </summary>
/// <param name="image">原图</param>
/// <returns>灰度化图片</returns>
std::shared_ptr<Image> ImageGrayscale::grayscale(std::shared_ptr<Image> image)
{
	//灰度化前后图片大小不变
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
/// 对像素的RGB值求平均即为其灰度值
/// </summary>
/// <param name="pixel">原像素</param>
/// <returns>灰度化像素</returns>
ImagePixel ImageGrayscale::grayscale(ImagePixel pixel)
{
	//对RGB值求平均即为灰度值
	BYTE grayValue = (pixel.R + pixel.G + pixel.B) / 3;
	return ImagePixel(pixel.A, grayValue, grayValue, grayValue);
}
