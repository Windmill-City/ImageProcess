#include "pch.h"
#include "ImageFormat.h"

std::string ImageFormat::PropertyName = "ImageFormat";

ImageFormat::ImageFormat(std::wstring postfix, std::unique_ptr<ImageReader> reader, std::unique_ptr<ImageWriter> writer)
{
	Ext = std::move(postfix);
	Reader = std::move(reader);
	Writer = std::move(writer);
}
