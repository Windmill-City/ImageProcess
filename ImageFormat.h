#pragma once
#include <string>
#include <memory>
#include "ImageProperty.h"
#include "ImageReader.h"
#include "ImageWriter.h"

class ImageFormat
{
public:
	static std::string PropertyName;
public:
	std::wstring Ext;
	std::unique_ptr<ImageReader> Reader;
	std::unique_ptr<ImageWriter> Writer;

	ImageFormat(std::wstring ext, std::unique_ptr<ImageReader> reader, std::unique_ptr<ImageWriter> writer);
};

