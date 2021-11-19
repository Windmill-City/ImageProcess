#include "pch.h"
#include "ImageFFT.h"
#include <complex>

#define PI (3.1415926535)

void FT(std::complex<double>* TD, std::complex<double>* FD, int len) {
	int x, u;
	double angle;
	for (u = 0; u < len; u++)
	{
		FD[u] = 0;
		for (x = 0; x < len; x++)
		{
			angle = -2 * PI * u * x / len;
			FD[u] += TD[x] * std::complex<double>(cos(angle), sin(angle));
		}
		FD[u] /= len;
	}
}
