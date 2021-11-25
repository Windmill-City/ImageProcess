#pragma once
#include <memory>
#include <map>
#include <vector>
#include "ImagePixel.h"
#include "ImageProperty.h"

class Image
{
public:
	size_t Width;
	size_t Height;
	std::vector<ImagePixel> Pixels;
	std::map<std::string, std::shared_ptr<ImageProperty>> Properties;

	Image(size_t width, size_t height);
};

