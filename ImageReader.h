#pragma once
#include <string>
#include "Image.h"
class ImageReader
{
public:
	virtual std::shared_ptr<Image> read(std::wstring path) = 0;
};

