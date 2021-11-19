#include "pch.h"
#include "ImageFormat.h"

ImageFormat::ImageFormat(std::wstring postfix, std::unique_ptr<ImageReader> reader, std::unique_ptr<ImageWriter> writer)
{
	Ext = std::move(postfix);
	Reader = std::move(reader);
	Writer = std::move(writer);
}

std::string ImageFormatProperty::PropertyName = "Format";

ImageFormatProperty::ImageFormatProperty(std::shared_ptr<ImageFormat> format)
{
	Format = format;
}
