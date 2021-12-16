#pragma once
#include "Image.h"

class ImageTemplateAverageFilterContext
{
public:
	/// <summary>
	/// 模板A，B参数, 即原点对应的坐标
	/// </summary>
	const int32_t A;
	const int32_t B;
	/// <summary>
	/// 模板矩阵数据
	/// </summary>
	const std::vector<std::vector<int32_t>> Matrix;
	/// <summary>
	/// 模板各项参数求和
	/// </summary>
	const int32_t Sum;
public:
	ImageTemplateAverageFilterContext(std::vector<std::vector<int32_t>> matrix);
};

class ImageTemplateMedianFilterContext {
public:
	/// <summary>
	/// 模板A，B参数, 即原点对应的坐标
	/// </summary>
	const int32_t A;
	const int32_t B;
	/// <summary>
	/// 临近像素数目
	/// </summary>
	const int32_t NearByCount;
public:
	ImageTemplateMedianFilterContext(int32_t m, int32_t n);
};



class ImageTemplateFilter
{
public:
	static std::shared_ptr<Image> applyTemplateAverage(std::shared_ptr<Image> image, const ImageTemplateAverageFilterContext ctx);
	static std::shared_ptr<Image> applyTemplateMedian(std::shared_ptr<Image> image, const ImageTemplateMedianFilterContext ctx);
};

