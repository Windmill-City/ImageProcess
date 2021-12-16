#include "pch.h"

#include "ImageFilterTransform.h"
#include "FormattedException.h"

#include <algorithm>
#include <numeric>


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

			result->Pixels[h * result->Width + w].R = sumR / ctx.Sum;
			result->Pixels[h * result->Width + w].G = sumG / ctx.Sum;
			result->Pixels[h * result->Width + w].B = sumB / ctx.Sum;
		}
	}

	return result;
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

ImageTemplateMedianFilterContext::ImageTemplateMedianFilterContext(int32_t m, int32_t n) : A(m / 2), B(n / 2), NearByCount(m * n)
{
}
