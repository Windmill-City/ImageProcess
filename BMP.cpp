#include "pch.h"
#include <iostream>
#include <fstream>

//(BM)0x424D
#define BITMAP_FILE_TYPE 0x4D42
//BMP ͼ������
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
	//��ɫ�峤��length
	info.biClrUsed = (0x1 << info.biBitCount) & 0x0000FFFF;
	//ͼ�񳤶�bytes
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
			// ָ�����ص�(i,j)��ָ��
			B = lpBits + LineBytes * i + j * 3;
			G = B + 1;
			R = G + 1;

			*R = *G = *B = (*R + *G + *B) / 3;
		}
	}
}