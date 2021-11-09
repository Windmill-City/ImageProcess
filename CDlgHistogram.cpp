// CDlgHistogram.cpp: 实现文件
//

#include "pch.h"
#include "CFD188.h"
#include "CDlgHistogram.h"
#include "afxdialogex.h"


// CDlgHistogram 对话框

IMPLEMENT_DYNAMIC(CDlgHistogram, CDialogEx)

CDlgHistogram::CDlgHistogram(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Histogram, pParent)
{

}

CDlgHistogram::~CDlgHistogram()
{
}

void CDlgHistogram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHistogram, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgHistogram 消息处理程序
extern int HistogramDataR[256];
extern int HistogramDataG[256];
extern int HistogramDataB[256];
extern void Histogram();
extern BOOL IsGray();

BOOL CDlgHistogram::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Histogram();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

int maxValOf(int* arr, int size) {
	int max = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (max < arr[i]) {
			max = arr[i];
		}
	}
	return max;
}

void drawHistogram(int x, int y, int* data, CPaintDC* dc) {
	dc->Rectangle(x, y, x + 256 + 1, y + 200 + 1);

	int max = maxValOf(data, 256);

	for (size_t i = 0; i < 256; i++)
	{
		dc->MoveTo(i + x + 1, 200 + y);
		dc->LineTo(i + x + 1, 200 + y - (int)(data[i] * 200 / max));
	}
}


void CDlgHistogram::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	drawHistogram(20, 20, HistogramDataR, &dc);
	if (!IsGray()) {
		drawHistogram(40 + 256, 20, HistogramDataG, &dc);
		drawHistogram(60 + 256 * 2, 20, HistogramDataB, &dc);
	}

	// 不为绘图消息调用 CDialogEx::OnPaint()
}
