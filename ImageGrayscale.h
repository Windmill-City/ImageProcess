#pragma once
#include <memory>
#include "Image.h"
class ImageGrayscale
{
public:
	static std::shared_ptr<Image> grayscale(std::shared_ptr<Image> image);
	static ImagePixel grayscale(ImagePixel pixel);
};

