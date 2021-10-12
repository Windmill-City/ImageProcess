#include "pch.h"
#include <iostream>
#include <fstream>

//(BM)0x424D
#define BITMAP_FILE_TYPE 0x4D42
//BMP 图像数据
BITMAPINFO* lpBitsInfo = NULL;

void loadBMP(LPCTSTR path) {
	std::ifstream bmpFile(path, std::ios::in | std::ios::binary);

	if (!bmpFile.is_open()) {
		std::cout << "Failed to open bmp file" << std::endl;
	}

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER info;

	bmpFile.read((char*)&header, sizeof(header));
	bmpFile.read((char*)&info, sizeof(info));

	if (header.bfType != BITMAP_FILE_TYPE)
	{
		std::cout << "Invild File type" << std::endl;
		return;
	}
	//调色板长度length
	info.biClrUsed = (0x1 << info.biBitCount) & 0x0000FFFF;
	//图像长度bytes
	DWORD imageSize = (info.biWidth * info.biBitCount + 31) / 32 * 4 * info.biHeight;

	if (lpBitsInfo) free(lpBitsInfo);

	lpBitsInfo = (BITMAPINFO*)malloc(sizeof(info) + info.biClrUsed * sizeof(RGBQUAD) + imageSize);

	memcpy(lpBitsInfo, &info, sizeof(info));

	bmpFile.read((char*)lpBitsInfo + sizeof(info), (size_t)info.biClrUsed * sizeof(RGBQUAD) + imageSize);
	bmpFile.close();
}

void toGray() {
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;

	int LineBytes =
		(w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;

	BYTE* lpBits =
		(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i, j;
	BYTE* R, *G, *B;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			// 指向像素点(i,j)的指针
			B = lpBits + LineBytes * i + j * 3;
			G = B + 1;
			R = G + 1;

			*R = *G = *B = (*R + *G + *B) / 3;
		}
	}
}