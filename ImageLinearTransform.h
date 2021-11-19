#pragma once
#include "Image.h"
class ImageLinearTransform
{
public:
	static std::shared_ptr<Image> linear(double slope, int intercept, std::shared_ptr<Image> image);
};

