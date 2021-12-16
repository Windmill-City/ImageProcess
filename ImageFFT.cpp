#include "pch.h"
#include "ImageFFT.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include <future>

std::string ImageFFT::PropertyName = "FFT";

std::string ImageFFT::PropertyName_R = "FFT_R";
std::string ImageFFT::PropertyName_G = "FFT_G";
std::string ImageFFT::PropertyName_B = "FFT_B";

/// <summary>
/// Rader �׵µ�λ���㷨
/// </summary>
/// һά���ݵ�λ��, ���ݴ�С����Ϊ2��N�η���NΪ������
/// <param name="begin"></param>
/// <param name="end"></param>
void ImageFFT::reversal(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end)
{
	size_t N = (size_t)std::sqrt(std::distance(begin, end));
	size_t NV_2 = N / 2;
	size_t NM_1 = N - 1;

	for (size_t I = 0, J = 0, k; I < NM_1; I++, J += k)
	{
		if (I < J) swap(begin[I], begin[J]);

		for (k = NV_2; k <= J; J -= k, k /= 2);
	}
}

/// <summary>
/// ת��N*N����
/// </summary>
/// <param name="begin"></param>
/// <param name="end"></param>
void ImageFFT::transpose(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end)
{
	size_t N = (size_t)std::sqrt(std::distance(begin, end));

	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = i + 1; j < N; j++)
		{
			swap(begin[i * N + j], begin[j * N + i]);
		}
	}
}

void ImageFFT::normalize(std::vector<double>::iterator begin, std::vector<double>::iterator end)
{
	auto min = *std::min_element(begin, end);
	auto max = *std::max_element(begin, end);
	auto section = max - min;

	for (; begin != end; begin++)
	{
		*begin = (*begin - min) / section;
	}
}

/// <summary>
/// DIT��-2FFT�㷨(ɣ��-ͼ���㷨)
/// </summary>
/// һά����FFT, ���ݴ�С����Ϊ2��N�η���NΪ������
/// <param name="begin"></param>
/// <param name="end"></param>
void ImageFFT::fft(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end)
{
	reversal(begin, end);

	size_t N = std::distance(begin, end);
	size_t L = (size_t)std::log2(N);

	for (size_t M = 1; M <= L; M++)
	{
		size_t LE = (size_t)std::pow(2, M);
		size_t LE_1 = LE / 2;

		std::complex<double> W(std::cos(-(M_PI / LE_1)), std::sin(-(M_PI / LE_1)));
		std::complex<double> U(1.0, 0);

		for (size_t J = 0; J < LE_1; J++, U *= W)
		{
			for (size_t I = J; I < N; I += LE)
			{
				auto IP = I + LE_1;
				auto T = begin[IP] * U;
				begin[IP] = begin[I] - T;
				begin[I] += T;
			}
		}
	}
}

/// <summary>
/// һάIFFT�㷨
/// </summary>
/// <param name="begin"></param>
/// <param name="end"></param>
void ImageFFT::ifft(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end)
{
	size_t N = std::distance(begin, end);
	for (size_t i = 0; i < N; i++)
	{
		begin[i] = std::conj(begin[i]);
	}

	fft(begin, end);

	for (size_t i = 0; i < N; i++)
	{
		begin[i] = std::conj(begin[i]) / std::complex<double>((double)N, 0);
	}
}

/// <summary>
/// 2άN*N����FFT
/// </summary>
/// <param name="begin"></param>
/// <param name="end"></param>
void ImageFFT::fft2(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end)
{
	size_t N = (size_t)std::sqrt(std::distance(begin, end));

	//�б任
	for (size_t i = 0; i < N; i++)
	{
		fft(begin + i * N, begin + i * N + N);
	}

	//ת��
	transpose(begin, end);

	//�б任
	for (size_t i = 0; i < N; i++)
	{
		fft(begin + i * N, begin + i * N + N);
	}

	//ת��
	transpose(begin, end);
}

/// <summary>
/// 2άN*N����IFFT
/// </summary>
/// <param name="begin"></param>
/// <param name="end"></param>
void ImageFFT::ifft2(std::vector<std::complex<double>>::iterator begin, std::vector<std::complex<double>>::iterator end)
{
	size_t N = (size_t)std::sqrt(std::distance(begin, end));

	//�б任
	for (size_t i = 0; i < N; i++)
	{
		ifft(begin + i * N, begin + i * N + N);
	}

	//ת��
	transpose(begin, end);

	//�б任
	for (size_t i = 0; i < N; i++)
	{
		ifft(begin + i * N, begin + i * N + N);
	}

	//ת��
	transpose(begin, end);
}

std::shared_ptr<Image> ImageFFT::fft(std::shared_ptr<Image> image)
{
	std::shared_ptr<Image> result = std::make_shared<Image>(*image);

	std::vector<std::complex<double>> complexR(result->Width * result->Height);
	std::vector<std::complex<double>> complexG(result->Width * result->Height);
	std::vector<std::complex<double>> complexB(result->Width * result->Height);

	//������RGB����ת��Ϊ������ʽ��ͬʱ�������Ļ�����
	for (size_t h = 0; h < result->Height; h++)
	{
		for (size_t w = 0; w < result->Width; w++)
		{
			complexR[h * result->Width + w] = std::complex<double>(result->Pixels[h * result->Width + w].R * std::pow(-1, h + w), 0);
			complexG[h * result->Width + w] = std::complex<double>(result->Pixels[h * result->Width + w].G * std::pow(-1, h + w), 0);
			complexB[h * result->Width + w] = std::complex<double>(result->Pixels[h * result->Width + w].B * std::pow(-1, h + w), 0);
		}
	}

	auto R_future = std::async(std::launch::async, fft2, complexR.begin(), complexR.end());
	auto G_future = std::async(std::launch::async, fft2, complexG.begin(), complexG.end());
	auto B_future = std::async(std::launch::async, fft2, complexB.begin(), complexB.end());

	R_future.get();
	G_future.get();
	B_future.get();

	std::vector<double> modulusR(result->Width * result->Height);
	std::vector<double> modulusG(result->Width * result->Height);
	std::vector<double> modulusB(result->Width * result->Height);

	for (size_t h = 0; h < result->Height; h++)
	{
		for (size_t w = 0; w < result->Width; w++)
		{
			modulusR[h * result->Width + w] = abs(complexR[h * result->Width + w]);
			modulusG[h * result->Width + w] = abs(complexG[h * result->Width + w]);
			modulusB[h * result->Width + w] = abs(complexB[h * result->Width + w]);
		}
	}

	normalize(modulusR.begin(), modulusR.end());
	normalize(modulusG.begin(), modulusG.end());
	normalize(modulusB.begin(), modulusB.end());

	//��FFT���ӳ���ͼƬ
	for (size_t h = 0; h < result->Height; h++)
	{
		for (size_t w = 0; w < result->Width; w++)
		{
			result->Pixels[h * result->Width + w].R = (BYTE)(modulusR[h * result->Width + w] * 255 * 1e3);
			result->Pixels[h * result->Width + w].G = (BYTE)(modulusG[h * result->Width + w] * 255 * 1e3);
			result->Pixels[h * result->Width + w].B = (BYTE)(modulusB[h * result->Width + w] * 255 * 1e3);
		}
	}

	result->Properties[PropertyName] = std::make_shared<ImagePropertyOf<bool>>(true);
	result->Properties[PropertyName_R] = std::make_shared<ImagePropertyOf<std::vector<std::complex<double>>>>(std::move(complexR));
	result->Properties[PropertyName_G] = std::make_shared<ImagePropertyOf<std::vector<std::complex<double>>>>(std::move(complexG));
	result->Properties[PropertyName_B] = std::make_shared<ImagePropertyOf<std::vector<std::complex<double>>>>(std::move(complexB));

	return result;
}

std::shared_ptr<Image> ImageFFT::ifft(std::shared_ptr<Image> image)
{
	std::shared_ptr<Image> result = std::make_shared<Image>(*image);

	std::vector<std::complex<double>> complexR = static_cast<ImagePropertyOf<std::vector<std::complex<double>>>*>(result->Properties[PropertyName_R].get())->Value;
	std::vector<std::complex<double>> complexG = static_cast<ImagePropertyOf<std::vector<std::complex<double>>>*>(result->Properties[PropertyName_G].get())->Value;
	std::vector<std::complex<double>> complexB = static_cast<ImagePropertyOf<std::vector<std::complex<double>>>*>(result->Properties[PropertyName_B].get())->Value;

	auto R_future = std::async(std::launch::async, ifft2, complexR.begin(), complexR.end());
	auto G_future = std::async(std::launch::async, ifft2, complexG.begin(), complexG.end());
	auto B_future = std::async(std::launch::async, ifft2, complexB.begin(), complexB.end());

	R_future.get();
	G_future.get();
	B_future.get();

	std::vector<double> modulusR(result->Width * result->Height);
	std::vector<double> modulusG(result->Width * result->Height);
	std::vector<double> modulusB(result->Width * result->Height);

	for (size_t h = 0; h < result->Height; h++)
	{
		for (size_t w = 0; w < result->Width; w++)
		{
			modulusR[h * result->Width + w] = abs(complexR[h * result->Width + w]);
			modulusG[h * result->Width + w] = abs(complexG[h * result->Width + w]);
			modulusB[h * result->Width + w] = abs(complexB[h * result->Width + w]);
		}
	}

	//��IFFT���ӳ���ͼƬ��ͬʱ����ȥ���Ļ�����
	for (size_t h = 0; h < result->Height; h++)
	{
		for (size_t w = 0; w < result->Width; w++)
		{
			result->Pixels[h * result->Width + w].R = (BYTE)modulusR[h * result->Width + w];
			result->Pixels[h * result->Width + w].G = (BYTE)modulusG[h * result->Width + w];
			result->Pixels[h * result->Width + w].B = (BYTE)modulusB[h * result->Width + w];
		}
	}

	result->Properties[PropertyName] = std::make_shared<ImagePropertyOf<bool>>(false);

	return result;
}
