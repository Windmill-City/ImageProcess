#pragma once
#include "ImageFormat.h"
class BMPFormat :
    public ImageFormat
{
public:
	class BMPReader : public ImageReader
	{
	public:
		// ͨ�� ImageReader �̳�
		virtual std::shared_ptr<Image> read(std::wstring path) override;
	};
	class BMPWriter : public ImageWriter
	{
	public:
		// ͨ�� ImageWriter �̳�
		virtual void write(std::shared_ptr<Image> image, std::wstring path) override;
	};
public:
    BMPFormat() noexcept;
};

