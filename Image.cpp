#include "pch.h"
#include "Image.h"

Image::Image(size_t width, size_t height)
{
	Width = width;
	Height = height;
	Pixels = std::vector<ImagePixel>(Height * Width);
}