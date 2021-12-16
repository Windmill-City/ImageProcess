#pragma once
#include "Image.h"

class ImageTemplateAverageFilterContext
{
public:
	/// <summary>
	/// ģ��A��B����, ��ԭ���Ӧ������
	/// </summary>
	const int32_t A;
	const int32_t B;
	/// <summary>
	/// ģ���������
	/// </summary>
	const std::vector<std::vector<int32_t>> Matrix;
	/// <summary>
	/// ģ�����������
	/// </summary>
	const int32_t Sum;
public:
	ImageTemplateAverageFilterContext(std::vector<std::vector<int32_t>> matrix);
};

class ImageTemplateMedianFilterContext {
public:
	/// <summary>
	/// ģ��A��B����, ��ԭ���Ӧ������
	/// </summary>
	const int32_t A;
	const int32_t B;
	/// <summary>
	/// �ٽ�������Ŀ
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

