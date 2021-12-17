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

#include "ImageFFT.h"

enum class FilterType
{
	LowPass,
	HighPass
};

class ImageFreqDomainFilterContext {
	const size_t Width, Height;
protected:
	const FilterType Type;
	const int32_t D0;
	double getD(int32_t u, int32_t v) const;
public:
	ImageFreqDomainFilterContext(FilterType type, size_t width, size_t height, int32_t d0);
public:
	virtual std::complex<double> getH(int32_t u, int32_t v) const = 0;
public:
	std::shared_ptr<Image> getFilterImage();
};

class IdealFilterContext : public ImageFreqDomainFilterContext {
public:
	IdealFilterContext(FilterType type, size_t width, size_t height, int32_t d0);
public:
	// ͨ�� ImageFreqDomainFilterContext �̳�
	virtual std::complex<double> getH(int32_t u, int32_t v) const override;
};

class ButterworthFilterContext : public ImageFreqDomainFilterContext {
	const int32_t N;
public:
	ButterworthFilterContext(int32_t n, FilterType type, size_t width, size_t height, int32_t d0);
public:
	// ͨ�� ImageFreqDomainFilterContext �̳�
	virtual std::complex<double> getH(int32_t u, int32_t v) const override;
};

class ImageFreqDomainFilter {
public:
	static std::shared_ptr<Image> applyFreqDomainFilter(std::shared_ptr<Image> image, const ImageFreqDomainFilterContext& ctx);
};

