// CDlgHistogram.cpp: 实现文件
//

#include "pch.h"
#include "CFD188.h"
#include "CDlgHistogram.h"
#include "afxdialogex.h"

#include "ImageHistogram.h"
#include "ImageLinearTransform.h"


// CDlgHistogram 对话框

IMPLEMENT_DYNAMIC(CDlgHistogram, CDialogEx)

CDlgHistogram::CDlgHistogram(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Histogram, pParent)
{
}

CDlgHistogram::~CDlgHistogram()
{
}

void CDlgHistogram::setView(CCFD188View* pView)
{
	View = pView;
	ImageOriginal = std::make_shared<Image>(*View->GetDocument()->ActiveImage);
}

#include <math.h>

void CDlgHistogram::applyLinearTrans()
{
	PosLeft = 255 - PointLeft.GetPos();
	PosRight = 255 - PointRight.GetPos();
	//斜率
	auto slope = (double)(PosRight - PosLeft) / 255;
	//截距
	auto intercept = PosLeft;

	View->GetDocument()->ActiveImage = ImageLinearTransform::linear(slope, intercept, ImageOriginal);
	View->Invalidate();

	Invalidate();
}

void CDlgHistogram::refreshHistogram()
{
	switch (ChannelSelector.GetCurSel())
	{
		//Grayscale
	case 0:
		Histogram = ImageHistogram::getHistogramGrayscale(ImageOriginal);
		break;
		//Red
	case 1:
		Histogram = ImageHistogram::getHistogramRed(ImageOriginal);
		break;
		//Green
	case 2:
		Histogram = ImageHistogram::getHistogramGreen(ImageOriginal);
		break;
		//Blue
	case 3:
		Histogram = ImageHistogram::getHistogramBlue(ImageOriginal);
		break;
	default:
		break;
	}
}

void CDlgHistogram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Channel_Select, ChannelSelector);
	DDX_Control(pDX, IDC_Point_Left, PointLeft);
	DDX_Control(pDX, IDC_Point_Right, PointRight);

	ChannelSelector.InsertString(0, L"Grayscale");
	ChannelSelector.InsertString(1, L"Red");
	ChannelSelector.InsertString(2, L"Green");
	ChannelSelector.InsertString(3, L"Blue");

	ChannelSelector.SetCurSel(0);
	refreshHistogram();

	PointLeft.SetRange(0, 255);
	PointLeft.SetLineSize(1);
	PointLeft.SetPageSize(1);
	PointLeft.SetPos(255);

	PointRight.SetRange(0, 255);
	PointRight.SetLineSize(1);
	PointRight.SetPageSize(1);
	PointRight.SetPos(0);
}


BEGIN_MESSAGE_MAP(CDlgHistogram, CDialogEx)
	ON_CBN_SELCHANGE(IDC_Channel_Select, &CDlgHistogram::OnCbnSelchangeChannelSelect)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &CDlgHistogram::OnBnClickedCancel)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CDlgHistogram 消息处理程序

#include <algorithm>

void CDlgHistogram::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//直方图左上坐标
	int graphX = 30;
	int graphY = 60;

	//直方图高度
	int graphHeight = 256;

	//边框
	dc.Rectangle(graphX, graphY, graphX + (int)Histogram.size() + 1, graphY + graphHeight);

	//高度缩放
	float scale = (float)(graphHeight - 1) / *std::max_element(Histogram.begin(), Histogram.end());

	//绘制斜线
	dc.MoveTo(graphX, graphY + PointLeft.GetPos());
	dc.LineTo(graphX + (int)Histogram.size() + 1, graphY + PointRight.GetPos());
	
	//边框占1像素
	graphX++;

	//绘制直方图
	for (size_t i = 0; i < Histogram.size(); i++)
	{
		int scaled = (int)(Histogram[i] * scale);
		dc.MoveTo(graphX + i, graphY + graphHeight - scaled);
		dc.LineTo(graphX + i, graphY + graphHeight);
	}
}

void CDlgHistogram::OnCbnSelchangeChannelSelect()
{
	refreshHistogram();
	Invalidate();
}


void CDlgHistogram::OnBnClickedCancel()
{
	View->GetDocument()->ActiveImage = ImageOriginal;
	View->Invalidate();
	CDialogEx::OnCancel();
}

void CDlgHistogram::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	applyLinearTrans();

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
