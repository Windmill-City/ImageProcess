#include "pch.h"
#include "ImageLinearTransform.h"

std::shared_ptr<Image> ImageLinearTransform::linear(double slope, int intercept, std::shared_ptr<Image> image)
{
    auto result = std::make_shared<Image>(*image);

	for (size_t h = 0; h < image->Height; h++)
	{
		for (size_t w = 0; w < image->Width; w++)
		{
			ImagePixel pixel = image->Pixels[h * image->Height + w];

			//ÏßÐÔ±ä»»
			pixel.R = (BYTE)(pixel.R * slope);
			pixel.G = (BYTE)(pixel.G * slope);
			pixel.B = (BYTE)(pixel.B * slope);

			pixel.R = (BYTE)(pixel.R + intercept);
			pixel.G = (BYTE)(pixel.G + intercept);
			pixel.B = (BYTE)(pixel.B + intercept);

			result->Pixels[h * image->Height + w] = pixel;
		}
	}

    return result;
}
