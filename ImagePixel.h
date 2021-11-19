#pragma once
struct ImagePixel
{
public:
	BYTE B;
	BYTE G;
	BYTE R;
	BYTE A;

	ImagePixel();
	ImagePixel(const BYTE alpha, const BYTE red, const BYTE green, const BYTE blue);
	ImagePixel(const RGBQUAD quad);
};

