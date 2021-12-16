
// CFD188View.cpp: CCFD188View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CFD188.h"
#endif

#include "CFD188Doc.h"
#include "CFD188View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCFD188View

IMPLEMENT_DYNCREATE(CCFD188View, CScrollView)

BEGIN_MESSAGE_MAP(CCFD188View, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_GREY, &CCFD188View::OnGrey)
	ON_UPDATE_COMMAND_UI(ID_GREY, &CCFD188View::OnUpdateGrey)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_RELOAD, &CCFD188View::OnReload)
	ON_UPDATE_COMMAND_UI(ID_RELOAD, &CCFD188View::OnUpdateReload)
	ON_COMMAND(ID_Histogram, &CCFD188View::OnHistogram)
	ON_UPDATE_COMMAND_UI(ID_Histogram, &CCFD188View::OnUpdateHistogram)
	ON_COMMAND(ID_EQUALIZE, &CCFD188View::OnEqualize)
	ON_UPDATE_COMMAND_UI(ID_EQUALIZE, &CCFD188View::OnUpdateEqualize)
	ON_COMMAND(ID_FFT, &CCFD188View::OnFFT)
	ON_UPDATE_COMMAND_UI(ID_FFT, &CCFD188View::OnUpdateFFT)
	ON_COMMAND(ID_AVG_FILTER, &CCFD188View::OnAvgFilter)
	ON_UPDATE_COMMAND_UI(ID_AVG_FILTER, &CCFD188View::OnUpdateAvgFilter)
	ON_COMMAND(ID_MID_FILTER, &CCFD188View::OnMidFilter)
	ON_UPDATE_COMMAND_UI(ID_MID_FILTER, &CCFD188View::OnUpdateMidFilter)
	ON_COMMAND(ID_GAUSS, &CCFD188View::OnGauss)
	ON_UPDATE_COMMAND_UI(ID_GAUSS, &CCFD188View::OnUpdateGauss)
	ON_COMMAND(ID_Laplace, &CCFD188View::OnLaplace)
	ON_UPDATE_COMMAND_UI(ID_Laplace, &CCFD188View::OnUpdateLaplace)
	ON_COMMAND(ID_Laplace_Normal, &CCFD188View::OnLaplaceNormal)
	ON_UPDATE_COMMAND_UI(ID_Laplace_Normal, &CCFD188View::OnUpdateLaplaceNormal)
END_MESSAGE_MAP()

// CCFD188View 构造/析构

CCFD188View::CCFD188View() noexcept
{
}

CCFD188View::~CCFD188View()
{
}

BOOL CCFD188View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

// CCFD188View 绘图
void CCFD188View::OnDraw(CDC* pDC)
{
	CCFD188Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	auto image = pDoc->ActiveImage;
	if (!image) return;

	//内存DC, 双缓冲[内存DC-设备DC]
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);

	//绘图区域
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, (int)image->Width, (int)image->Height);

	//绘图区域属性
	BITMAP bm;
	bitmap.GetObject(sizeof(bm), &bm);

	//图像数据->绘图区域
	bitmap.SetBitmapBits(bm.bmWidthBytes * bm.bmHeight, image->Pixels.data());

	//绘图区域->内存DC, 在内存中绘制
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);

	//绘制区域->设备DC, 显示绘制结果
	pDC->BitBlt(0, 0, (int)image->Width, (int)image->Height, &memDC, 0, 0, SRCCOPY);

	//释放位图对象, 避免资源泄露
	memDC.SelectObject(pOldBmp);
}

void CCFD188View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	std::shared_ptr<Image> image = GetDocument()->ActiveImage;
	if (image) {
		sizeTotal.cx = (LONG)image->Width;
		sizeTotal.cy = (LONG)image->Height;
	}
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CCFD188View 打印

BOOL CCFD188View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCFD188View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCFD188View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CCFD188View 诊断

#ifdef _DEBUG
void CCFD188View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCFD188View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCFD188Doc* CCFD188View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCFD188Doc)));
	return (CCFD188Doc*)m_pDocument;
}
#endif //_DEBUG


// CCFD188View 消息处理程序

#include "MainFrm.h"
#include "StringUtils.h"

void CCFD188View::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);

	std::shared_ptr<Image> image = GetDocument()->ActiveImage;
	if (!image || point.x >= image->Width || point.y >= image->Height) return;

	auto pixel = image->Pixels[(size_t)point.y * point.x];
	auto pixelInfo = StringUtils::format("ARGB(%d, %d, %d, %d)", pixel.A, pixel.R, pixel.G, pixel.B);

	((CMainFrame*)GetParent())->SetMessageText(StringUtils::convert(pixelInfo.c_str()).c_str());
}

#include "ImageGrayscale.h"

void CCFD188View::OnGrey()
{
	auto pDoc = GetDocument();
	pDoc->ActiveImage = ImageGrayscale::grayscale(pDoc->ActiveImage);
	Invalidate();
	pDoc->SetModifiedFlag();
}

void CCFD188View::OnUpdateGrey(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->ActiveImage != nullptr);
}

#include "ImageEqualize.h"

void CCFD188View::OnEqualize()
{
	auto pDoc = GetDocument();
	pDoc->ActiveImage = ImageEqualize::equalize(pDoc->ActiveImage);
	Invalidate();
	pDoc->SetModifiedFlag();
}


void CCFD188View::OnUpdateEqualize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->ActiveImage != nullptr);
}

void CCFD188View::OnReload()
{
	auto pDoc = GetDocument();
	pDoc->ActiveImage = pDoc->ActiveImageOriginal;
	Invalidate();
	pDoc->SetModifiedFlag(false);
}


void CCFD188View::OnUpdateReload(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->ActiveImageOriginal != nullptr);
}

#include "CDlgHistogram.h"

void CCFD188View::OnHistogram()
{
	CDlgHistogram dlg;
	dlg.setView(this);
	dlg.DoModal();
}


void CCFD188View::OnUpdateHistogram(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->ActiveImage != nullptr);
}

#include "ImageFFT.h"

void CCFD188View::OnFFT()
{
	auto pDoc = GetDocument();
	auto propFFT = static_cast<ImagePropertyOf<bool>*>(pDoc->ActiveImage->Properties[ImageFFT::PropertyName].get());
	pDoc->ActiveImage = propFFT && propFFT->Value ? ImageFFT::ifft(pDoc->ActiveImage) : ImageFFT::fft(pDoc->ActiveImage);
	Invalidate();
	pDoc->SetModifiedFlag();
}


void CCFD188View::OnUpdateFFT(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->ActiveImage != nullptr);
}

#include "ImageFilterTransform.h"

void CCFD188View::OnAvgFilter()
{
	auto pDoc = GetDocument();
	pDoc->ActiveImage = ImageTemplateFilter::applyTemplateAverage(pDoc->ActiveImage,
		ImageTemplateAverageFilterContext(std::vector<std::vector<int32_t>>(3, std::vector<int32_t>(3, 1))));
	Invalidate();
	pDoc->SetModifiedFlag();
}


void CCFD188View::OnUpdateAvgFilter(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->ActiveImage != nullptr);
}


void CCFD188View::OnMidFilter()
{
	auto pDoc = GetDocument();
	pDoc->ActiveImage = ImageTemplateFilter::applyTemplateMedian(pDoc->ActiveImage, ImageTemplateMedianFilterContext(3, 3));
	Invalidate();
	pDoc->SetModifiedFlag();
}


void CCFD188View::OnUpdateMidFilter(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->ActiveImage != nullptr);
}


void CCFD188View::OnGauss()
{
	auto pDoc = GetDocument();
	//高斯滤波模板
	auto matrix = std::vector<std::vector<int32_t>>(3, std::vector<int32_t>(3, 1));

	matrix[1][1] = 4;

	matrix[0][1] = 2;
	matrix[1][0] = 2;
	matrix[1][2] = 2;
	matrix[2][1] = 2;

	pDoc->ActiveImage = ImageTemplateFilter::applyTemplateAverage(pDoc->ActiveImage,
		ImageTemplateAverageFilterContext(matrix));
	Invalidate();
	pDoc->SetModifiedFlag();
}


void CCFD188View::OnUpdateGauss(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->ActiveImage != nullptr);
}


void CCFD188View::OnLaplace()
{
	auto pDoc = GetDocument();
	//拉普拉斯滤波模板
	auto matrix = std::vector<std::vector<int32_t>>(3, std::vector<int32_t>(3, 0));

	matrix[1][1] = 5;

	matrix[0][1] = -1;
	matrix[1][0] = -1;
	matrix[1][2] = -1;
	matrix[2][1] = -1;

	pDoc->ActiveImage = ImageTemplateFilter::applyTemplateAverage(pDoc->ActiveImage,
		ImageTemplateAverageFilterContext(matrix));
	Invalidate();
	pDoc->SetModifiedFlag();
}


void CCFD188View::OnUpdateLaplace(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->ActiveImage != nullptr);
}


void CCFD188View::OnLaplaceNormal()
{
	auto pDoc = GetDocument();
	//拉普拉斯滤波模板
	auto matrix = std::vector<std::vector<int32_t>>(3, std::vector<int32_t>(3, -1));

	matrix[1][1] = 9;

	pDoc->ActiveImage = ImageTemplateFilter::applyTemplateAverage(pDoc->ActiveImage,
		ImageTemplateAverageFilterContext(matrix));
	Invalidate();
	pDoc->SetModifiedFlag();
}


void CCFD188View::OnUpdateLaplaceNormal(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->ActiveImage != nullptr);
}
