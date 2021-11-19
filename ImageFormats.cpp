#include "pch.h"
#include <stdlib.h>
#include <algorithm>
#include "BMPFormat.h"
#include "ImageFormats.h"
#include "FormattedException.h"

void ImageFormats::initialize()
{
	Formats.push_back(std::make_shared<BMPFormat>());
}

std::shared_ptr<Image> ImageFormats::read(std::wstring path)
{
	WCHAR ext[16];
	_wsplitpath_s(path.c_str(), NULL, 0, NULL, 0, NULL, 0, ext, 16);

	auto extStr = std::wstring(ext);
	std::transform(extStr.begin(), extStr.end(), extStr.begin(), std::tolower);

	for (size_t i = 0; i < Formats.size(); i++)
	{
		if (Formats[i]->Ext.compare(extStr) == 0) {
			auto image = Formats[i]->Reader->read(path);
			image->Properties.insert(std::make_pair(ImageFormatProperty::PropertyName, ImageFormatProperty(Formats[i])));
			return image;
		}
	}

	throw UnsupportImageFormatException(extStr);
}

void ImageFormats::write(std::shared_ptr<Image> image, std::wstring path)
{
	WCHAR ext[16];
	_wsplitpath_s(path.c_str(), NULL, 0, NULL, 0, NULL, 0, ext, 16);

	auto extStr = std::wstring(ext);
	std::transform(extStr.begin(), extStr.end(), extStr.begin(), std::tolower);

	for (size_t i = 0; i < Formats.size(); i++)
	{
		if (Formats[i]->Ext.compare(extStr) == 0) {
			Formats[i]->Writer->write(image, path);
			return;
		}
	}

	throw UnsupportImageFormatException(extStr);
}

UnsupportImageFormatException::UnsupportImageFormatException(std::wstring ext) : FormattedException("Unsupport ImageFormat: %ls", ext.c_str())
{
	Ext = ext;
}
