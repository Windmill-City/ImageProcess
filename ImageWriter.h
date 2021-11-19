#pragma once
#include <string>
#include "Image.h"
class ImageWriter
{
public:
	virtual void write(std::shared_ptr<Image> image, std::wstring path) = 0;
};

