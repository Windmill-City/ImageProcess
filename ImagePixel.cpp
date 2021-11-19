#include "pch.h"
#include "ImagePixel.h"

ImagePixel::ImagePixel()
{
	A = 0;
	R = 0;
	G = 0;
	B = 0;
}

ImagePixel::ImagePixel(const BYTE alpha, const BYTE red, const BYTE green, const BYTE blue)
{
	A = alpha;
	R = red;
	G = green;
	B = blue;
}

ImagePixel::ImagePixel(const RGBQUAD quad) :ImagePixel(255, quad.rgbRed, quad.rgbGreen, quad.rgbBlue)
{
}
