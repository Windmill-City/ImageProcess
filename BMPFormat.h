#pragma once
#include "ImageFormat.h"
class BMPFormat :
    public ImageFormat
{
public:
	class BMPReader : public ImageReader
	{
	public:
		// 通过 ImageReader 继承
		virtual std::shared_ptr<Image> read(std::wstring path) override;
	};
	class BMPWriter : public ImageWriter
	{
	public:
		// 通过 ImageWriter 继承
		virtual void write(std::shared_ptr<Image> image, std::wstring path) override;
	};
public:
    BMPFormat() noexcept;
};

