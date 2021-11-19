#pragma once
#include <memory>
#include "Image.h"
class ImageEqualize
{
public:
	static std::shared_ptr<Image> equalize(std::shared_ptr<Image> image);
};

