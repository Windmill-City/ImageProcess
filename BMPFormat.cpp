#include "pch.h"
#include <fstream>
#include "FormattedException.h"
#include "BMPFormat.h"

//(BM)0x424D
#define BITMAP_FILE_HEADER 0x4D42

BMPFormat::BMPFormat() noexcept: ImageFormat(std::wstring(L".bmp"), std::make_unique<BMPReader>(), std::make_unique<BMPWriter>())
{
}

std::shared_ptr<Image> BMPFormat::BMPReader::read(std::wstring path)
{
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file) {
		throw FormattedException("Failed to open file!");
	}

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER info;

	file.read((char*)&header, sizeof(header));
	file.read((char*)&info, sizeof(info));

	if (header.bfType != BITMAP_FILE_HEADER) {
		throw FormattedException("Invalid bitmap file header!");
	}

	//若调色板长度为0，则需要计算调色板长度
	info.biClrUsed = info.biClrUsed ? info.biClrUsed : (0x1 << info.biBitCount) & 0x0000FFFF;

	auto colors = info.biClrUsed ? std::vector<RGBQUAD>(info.biClrUsed) : std::vector<RGBQUAD>();
	if(info.biClrUsed)
		file.read((char*)&colors[0], sizeof(RGBQUAD) * (QWORD)info.biClrUsed);

	auto image = std::make_shared<Image>(info.biWidth, info.biHeight);

	//行数据长度
	auto lineBytesCount = (((image->Width * info.biBitCount) + 31) >> 5) << 2;
	//行数据
	auto lineBytes = std::make_unique<BYTE[]>(lineBytesCount);
	//像素数据掩膜
	BYTE mask = (0x0001 << info.biBitCount) - 1;

	for (signed int h = (signed int)(image->Height - 1); h >= 0; h--)
	{
		file.read((char*)lineBytes.get(), lineBytesCount);
		for (size_t w = 0; w < image->Width; w++)
		{
			switch (info.biBitCount)
			{
			case 1:
			case 4:
			case 8:
				image->Pixels[h * image->Height + w] = (ImagePixel)colors[
					(lineBytes[w / (8 / info.biBitCount)]
						>> ((8 - info.biBitCount) - (w % (8 / info.biBitCount) * info.biBitCount)))
						& mask];
				break;
			case 24:
				image->Pixels[h * image->Height + w] = *(ImagePixel*)(&lineBytes[w * (24 / 8)]);
				break;
			default:
				throw FormattedException("Invalid bitmap bitcount: %d", info.biBitCount);
			}
		}
	}

	return image;
}

void BMPFormat::BMPWriter::write(std::shared_ptr<Image> image, std::wstring path)
{

}
