#pragma once
#include <memory>
#include "Image.h"
class ImageHistogram
{
public:
	static std::vector<size_t> getHistogramGrayscale(std::shared_ptr<Image> image);
	static std::vector<size_t> getHistogramRed(std::shared_ptr<Image> image);
	static std::vector<size_t> getHistogramGreen(std::shared_ptr<Image> image);
	static std::vector<size_t> getHistogramBlue(std::shared_ptr<Image> image);
};

