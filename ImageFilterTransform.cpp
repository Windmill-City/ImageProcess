#include "pch.h"

#include <numeric>
#include <algorithm>

#include "ImageFilterTransform.h"
#include "FormattedException.h"


ImageTemplateAverageFilterContext::ImageTemplateAverageFilterContext(std::vector<std::vector<int32_t>> matrix) : Matrix(matrix),
A((int32_t)matrix.size() / 2), B((int32_t)matrix.at(0).size() / 2),
Sum(std::accumulate(matrix.begin(), matrix.end(), 0, [](auto lhs, const auto& rhs) {return std::accumulate(rhs.begin(), rhs.end(), lhs);}))
{
}

std::shared_ptr<Image> ImageTemplateFilter::applyTemplateAverage(std::shared_ptr<Image> image, const ImageTemplateAverageFilterContext ctx)
{
	std::shared_ptr<Image> result = std::make_shared<Image>(*image);

	for (int32_t h = 0; h < result->Height; h++)
	{
		for (int32_t w = 0; w < result->Width; w++)
		{
			//模板超出图像范围，跳过处理
			if (w - ctx.A < 0 || w + ctx.A >= result->Width
				|| h - ctx.B < 0 || h + ctx.B >= result->Height)
				continue;

			int32_t sumR = 0, sumG = 0, sumB = 0;

			for (int32_t s = -ctx.A; s <= ctx.A; s++)
			{
				for (int32_t t = -ctx.B; t <= ctx.B; t++)
				{
					auto nearbyPixel = image->Pixels[(h + t) * result->Width + (w + s)];
					auto templateValue = ctx.Matrix[ctx.B + t][ctx.A + s];

					sumR += nearbyPixel.R * templateValue;
					sumG += nearbyPixel.G * templateValue;
					sumB += nearbyPixel.B * templateValue;
				}
			}

			result->Pixels[h * result->Width + w].R = std::clamp(sumR / ctx.Sum, 0, 255);
			result->Pixels[h * result->Width + w].G = std::clamp(sumG / ctx.Sum, 0, 255);
			result->Pixels[h * result->Width + w].B = std::clamp(sumB / ctx.Sum, 0, 255);
		}
	}

	return result;
}

ImageTemplateMedianFilterContext::ImageTemplateMedianFilterContext(int32_t m, int32_t n) : A(m / 2), B(n / 2), NearByCount(m* n)
{
}

std::shared_ptr<Image> ImageTemplateFilter::applyTemplateMedian(std::shared_ptr<Image> image, const ImageTemplateMedianFilterContext ctx)
{
	std::shared_ptr<Image> result = std::make_shared<Image>(*image);

	for (int32_t h = 0; h < result->Height; h++)
	{
		for (int32_t w = 0; w < result->Width; w++)
		{
			//模板超出图像范围，跳过处理
			if (w - ctx.A < 0 || w + ctx.A >= result->Width
				|| h - ctx.B < 0 || h + ctx.B >= result->Height)
				continue;

			auto nearbyR = std::vector<BYTE>(ctx.NearByCount);
			auto nearbyG = std::vector<BYTE>(ctx.NearByCount);
			auto nearbyB = std::vector<BYTE>(ctx.NearByCount);

			for (int32_t s = -ctx.A, i = 0; s <= ctx.A; s++)
			{
				for (int32_t t = -ctx.B; t <= ctx.B; t++)
				{
					auto nearbyPixel = image->Pixels[(h + t) * result->Width + (w + s)];

					nearbyR[i] = nearbyPixel.R;
					nearbyG[i] = nearbyPixel.G;
					nearbyB[i] = nearbyPixel.B;
					i++;
				}
			}

			//中值排序
			std::nth_element(nearbyR.begin(), nearbyR.begin() + ctx.NearByCount / 2, nearbyR.end());
			std::nth_element(nearbyG.begin(), nearbyG.begin() + ctx.NearByCount / 2, nearbyG.end());
			std::nth_element(nearbyB.begin(), nearbyB.begin() + ctx.NearByCount / 2, nearbyB.end());

			result->Pixels[h * result->Width + w].R = nearbyR[ctx.NearByCount / 2];
			result->Pixels[h * result->Width + w].G = nearbyG[ctx.NearByCount / 2];
			result->Pixels[h * result->Width + w].B = nearbyB[ctx.NearByCount / 2];
		}
	}

	return result;
}

ImageFreqDomainFilterContext::ImageFreqDomainFilterContext(FilterType type, size_t width, size_t height, int32_t d0) : Type(type), Width(width), Height(height), D0(d0)
{
}

std::shared_ptr<Image> ImageFreqDomainFilterContext::getFilterImage()
{
	auto result = std::make_shared<Image>(Width, Height);

	for (int32_t h = 0; h < result->Height; h++)
	{
		for (int32_t w = 0; w < result->Width; w++)
		{
			result->Pixels[h * result->Width + w].A = 255;
			result->Pixels[h * result->Width + w].R = abs(getH(w, h)) * 255;
			result->Pixels[h * result->Width + w].G = abs(getH(w, h)) * 255;
			result->Pixels[h * result->Width + w].B = abs(getH(w, h)) * 255;
		}
	}

	return result;
}

double ImageFreqDomainFilterContext::getD(int32_t u, int32_t v) const
{
	return std::sqrt(std::pow(u - (double)Width / 2, 2.0) + std::pow(v - (double)Height / 2, 2.0));
}

IdealFilterContext::IdealFilterContext(FilterType type, size_t width, size_t height, int32_t d0) : ImageFreqDomainFilterContext(type, width, height, d0)
{
}

std::complex<double> IdealFilterContext::getH(int32_t u, int32_t v) const
{
	if ((Type == FilterType::LowPass && getD(u, v) < D0) || (Type == FilterType::HighPass && getD(u, v) > D0))
		return std::complex<double>(1, 0);
	else
		return std::complex<double>(0, 0);
}

ButterworthFilterContext::ButterworthFilterContext(int32_t n, FilterType type, size_t width, size_t height, int32_t d0) : ImageFreqDomainFilterContext(type, width, height, d0), N(n)
{
}

std::complex<double> ButterworthFilterContext::getH(int32_t u, int32_t v) const
{
	if (Type == FilterType::LowPass)
		return std::complex<double>(1 / std::pow(1 + getD(u, v) / D0, 2 * N), 0);
	else
		return std::complex<double>(1 / std::pow(1 + D0 / getD(u, v), 2 * N), 0);
}

std::shared_ptr<Image> ImageFreqDomainFilter::applyFreqDomainFilter(std::shared_ptr<Image> image, const ImageFreqDomainFilterContext& ctx)
{
	auto result = ImageFFT::fft(image);

	auto complexR = static_cast<ImagePropertyOf<std::vector<std::complex<double>>>*>(result->Properties[ImageFFT::PropertyName_R].get())->Value;
	auto complexG = static_cast<ImagePropertyOf<std::vector<std::complex<double>>>*>(result->Properties[ImageFFT::PropertyName_G].get())->Value;
	auto complexB = static_cast<ImagePropertyOf<std::vector<std::complex<double>>>*>(result->Properties[ImageFFT::PropertyName_B].get())->Value;

	for (int32_t h = 0; h < result->Height; h++)
	{
		for (int32_t w = 0; w < result->Width; w++)
		{
			complexR[h * result->Width + w] *= ctx.getH(w, h);
			complexG[h * result->Width + w] *= ctx.getH(w, h);
			complexB[h * result->Width + w] *= ctx.getH(w, h);
		}
	}

	result->Properties[ImageFFT::PropertyName_R] = std::make_shared<ImagePropertyOf<std::vector<std::complex<double>>>>(std::move(complexR));
	result->Properties[ImageFFT::PropertyName_G] = std::make_shared<ImagePropertyOf<std::vector<std::complex<double>>>>(std::move(complexG));
	result->Properties[ImageFFT::PropertyName_B] = std::make_shared<ImagePropertyOf<std::vector<std::complex<double>>>>(std::move(complexB));

	return ImageFFT::ifft(result);
}
