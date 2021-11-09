#include "pch.h"
#include <iostream>
#include <fstream>

//(BM)0x424D
#define BITMAP_FILE_TYPE 0x4D42
//BMP 图像数据
BITMAPINFO* lpBitsInfo = NULL;
BITMAPINFO* lpBitsInfoOrigin = NULL;
DWORD bmOriginSize;

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
	info.biClrUsed = info.biClrUsed ? info.biClrUsed : (0x1 << info.biBitCount) & 0x0000FFFF;
	//图像长度bytes
	DWORD imageSize = (info.biWidth * info.biBitCount + 31) / 32 * 4 * info.biHeight;
	bmOriginSize = sizeof(info) + info.biClrUsed * sizeof(RGBQUAD) + imageSize;

	if (lpBitsInfo) free(lpBitsInfo);
	if (lpBitsInfoOrigin) free(lpBitsInfoOrigin);

	lpBitsInfo = (BITMAPINFO*)malloc(bmOriginSize);
	lpBitsInfoOrigin = (BITMAPINFO*)malloc(bmOriginSize);

	memcpy(lpBitsInfo, &info, sizeof(info));

	bmpFile.read((char*)lpBitsInfo + sizeof(info), (size_t)info.biClrUsed * sizeof(RGBQUAD) + imageSize);
	bmpFile.close();

	memcpy(lpBitsInfoOrigin, lpBitsInfo, bmOriginSize);
}

void toGray() {
	switch (lpBitsInfo->bmiHeader.biBitCount) {
	case 1:
	case 4:
	case 8:
		for (size_t i = 0; i < lpBitsInfo->bmiHeader.biClrUsed; i++)
		{
			BYTE R, G, B, avg;
			R = lpBitsInfo->bmiColors[i].rgbRed;
			G = lpBitsInfo->bmiColors[i].rgbGreen;
			B = lpBitsInfo->bmiColors[i].rgbBlue;
			avg = (R + G + B) / 3;
			lpBitsInfo->bmiColors[i].rgbRed = avg;
			lpBitsInfo->bmiColors[i].rgbGreen = avg;
			lpBitsInfo->bmiColors[i].rgbBlue = avg;
		}
		break;
	case 24:
		int w = lpBitsInfo->bmiHeader.biWidth;
		int h = lpBitsInfo->bmiHeader.biHeight;

		int LineBytes =
			(w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;

		BYTE* lpBits =
			(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

		int LineBytes_gray = (w * 8 + 31) / 32 * 4;
		BITMAPINFO* lpBitsInfo_gray = (BITMAPINFO*)malloc(LineBytes_gray * h + 40 + 1024);

		memcpy(lpBitsInfo_gray, lpBitsInfo, 40);
		lpBitsInfo_gray->bmiHeader.biBitCount = 8;
		lpBitsInfo_gray->bmiHeader.biClrUsed = 256;

		int i;
		for (i = 0; i < 256; i++) {
			lpBitsInfo_gray->bmiColors[i].rgbRed = i;
			lpBitsInfo_gray->bmiColors[i].rgbGreen = i;
			lpBitsInfo_gray->bmiColors[i].rgbBlue = i;
			lpBitsInfo_gray->bmiColors[i].rgbReserved = 0;
		}

		int j;
		BYTE* R, * G, * B, avg, * pixel;
		BYTE* lpBits_gray = (BYTE*)&lpBitsInfo_gray->bmiColors[256];
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				B = lpBits + LineBytes * i + j * 3;
				G = B + 1;
				R = G + 1;

				avg = *R = *G = *B = (*R + *G + *B) / 3;

				pixel = lpBits_gray + i * LineBytes_gray + j;
				*pixel = avg;
			}
		}

		free(lpBitsInfo);
		lpBitsInfo = lpBitsInfo_gray;
	}
}

BOOL IsGray() {
	int r, g, b;
	switch (lpBitsInfo->bmiHeader.biBitCount)
	{
	case 1:
		return true;
	case 4:
	case 8:
		int offset = lpBitsInfo->bmiHeader.biClrUsed/2;
		r = lpBitsInfo->bmiColors[offset].rgbRed;
		b = lpBitsInfo->bmiColors[offset].rgbBlue;
		g = lpBitsInfo->bmiColors[offset].rgbGreen;

		return r == b && r == g;
	}
	return false;
}

void pixel(int i, int j, char* str) {
	if (!lpBitsInfo)return;

	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;

	int LineBytes =
		(w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;

	BYTE* lpBits =
		(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	if (i >= h || j >= w)return;

	BYTE* pixel, r, g, b;
	switch (lpBitsInfo->bmiHeader.biBitCount)
	{
	case 1:
		pixel = lpBits + LineBytes * (h - 1 - i) + j / 8;
		sprintf_s(str, 100, "灰度值:%d", *pixel >> (7 - j % 8) & 0x01);
		break;
	case 4:
		pixel = lpBits + LineBytes * (h - 1 - i) + j / 2;

		if (IsGray())
			sprintf_s(str, 100, "灰度值:%d", *pixel >> j % 2 ? 4 : 0);
		else {
			r = lpBitsInfo->bmiColors[(*pixel >> (j % 2 ? 4 : 0)) & 0xF].rgbRed;
			g = lpBitsInfo->bmiColors[(*pixel >> (j % 2 ? 4 : 0)) & 0xF].rgbGreen;
			b = lpBitsInfo->bmiColors[(*pixel >> (j % 2 ? 4 : 0)) & 0xF].rgbBlue;

			sprintf_s(str, 100, "RGB(%d, %d, %d)", r, g, b);
		}
	case 8:
		pixel = lpBits + LineBytes * (h - 1 - i) + j;
		if (IsGray())
			sprintf_s(str, 100,"灰度值:%d", *pixel);
		else {
			r = lpBitsInfo->bmiColors[*pixel].rgbRed;
			g = lpBitsInfo->bmiColors[*pixel].rgbGreen;
			b = lpBitsInfo->bmiColors[*pixel].rgbBlue;

			sprintf_s(str, 100,"RGB(%d, %d, %d)", r, g, b);
		}
		break;
	case 24:
		b = *(lpBits + LineBytes * i + j * 3);
		g = *(lpBits + LineBytes * i + j * 3 + 1);
		r = *(lpBits + LineBytes * i + j * 3 + 2);

		sprintf_s(str, 100, "RGB(%d, %d, %d)", r, g, b);
		break;
	default:
		break;
	}
}

int HistogramDataR[256];
int HistogramDataG[256];
int HistogramDataB[256];
void Histogram() {
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;

	int LineBytes =
		(w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;

	BYTE* lpBits =
		(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	BYTE* pixel;

	ZeroMemory(HistogramDataR, sizeof(HistogramDataR));
	ZeroMemory(HistogramDataG, sizeof(HistogramDataG));
	ZeroMemory(HistogramDataB, sizeof(HistogramDataB));

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			BYTE pixelR;
			BYTE pixelG;
			BYTE pixelB;


			switch (lpBitsInfo->bmiHeader.biBitCount)
			{
			case 1:
				pixel = lpBits + LineBytes * (h - 1 - i) + j / 8;

				pixelB = (*pixel >> (7 - j % 8) & 0x01);
				pixelG = (*pixel >> (7 - j % 8) & 0x01);
				pixelR = (*pixel >> (7 - j % 8) & 0x01);
				break;
			case 4:
				pixel = lpBits + LineBytes * (h - 1 - i) + j / 2;

				pixelR = lpBitsInfo->bmiColors[(*pixel >> (j % 2 ? 4 : 0)) & 0xF].rgbRed;
				pixelG = lpBitsInfo->bmiColors[(*pixel >> (j % 2 ? 4 : 0)) & 0xF].rgbGreen;
				pixelB = lpBitsInfo->bmiColors[(*pixel >> (j % 2 ? 4 : 0)) & 0xF].rgbBlue;
				break;
			case 8:
				pixel = lpBits + LineBytes * (h - 1 - i) + j;

				pixelR = lpBitsInfo->bmiColors[*pixel].rgbRed;
				pixelG = lpBitsInfo->bmiColors[*pixel].rgbGreen;
				pixelB = lpBitsInfo->bmiColors[*pixel].rgbBlue;
				break;
			case 24:
				pixelB = *(lpBits + LineBytes * i + j * 3);
				pixelG = *(lpBits + LineBytes * i + j * 3 + 1);
				pixelR = *(lpBits + LineBytes * i + j * 3 + 2);
				break;
			}

			HistogramDataR[pixelR]++;
			HistogramDataG[pixelG]++;
			HistogramDataB[pixelB]++;
		}
	}
}