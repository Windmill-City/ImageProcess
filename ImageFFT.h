#pragma once
#include <complex>
#include <vector>
#include "Image.h"
class ImageFFT
{
public:
	static std::string PropertyName;
public:
	static void reversal(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end);
	static void transpose(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end);

	static void fft(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end);
	static void ifft(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end);

	static void fft2(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end);
	static void ifft2(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end);

	static std::shared_ptr<Image> fft(std::shared_ptr<Image> image);
	static std::shared_ptr<Image> ifft(std::shared_ptr<Image> image);
};

