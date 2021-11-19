#pragma once
#include <vector>
#include "FormattedException.h"
#include "ImageFormat.h"
class ImageFormats
{
public:
	std::vector<std::shared_ptr<ImageFormat>> Formats;
public:
	void initialize();
	std::shared_ptr<Image> read(std::wstring path);
	void write(std::shared_ptr<Image> image, std::wstring path);
};

class UnsupportImageFormatException : public FormattedException
{
public:
	std::wstring Ext;
public:
	UnsupportImageFormatException(std::wstring ext);
};


