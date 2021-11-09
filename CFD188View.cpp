
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
END_MESSAGE_MAP()

// CCFD188View 构造/析构

CCFD188View::CCFD188View() noexcept
{
	// TODO: 在此处添加构造代码

}

CCFD188View::~CCFD188View()
{
}

BOOL CCFD188View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CCFD188View 绘图
//BMP 图像数据
extern BITMAPINFO* lpBitsInfo;
void CCFD188View::OnDraw(CDC* pDC)
{
	CCFD188Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (!lpBitsInfo) return;
	LPVOID lpBits = (LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	StretchDIBits(pDC->m_hDC,
		0, 0, lpBitsInfo->bmiHeader.biWidth, lpBitsInfo->bmiHeader.biHeight,
		0, 0, lpBitsInfo->bmiHeader.biWidth, lpBitsInfo->bmiHeader.biHeight,
		lpBits, lpBitsInfo, DIB_RGB_COLORS, SRCCOPY);
}

void CCFD188View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
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

void toGray();
void CCFD188View::OnGrey()
{
	toGray();
	Invalidate();
}

BOOL IsGray();
void CCFD188View::OnUpdateGrey(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(lpBitsInfo != NULL);
}

void pixel(int i, int j, char* str);
#include "MainFrm.h"

void CCFD188View::OnMouseMove(UINT nFlags, CPoint point)
{
	char xy[100];
	ZeroMemory(xy, sizeof(xy));
	sprintf_s(xy, sizeof(xy)/sizeof(char),"X:%d Y:%d    ", point.x, point.y);

	char rgb[100]; 
	ZeroMemory(rgb, sizeof(rgb));
	pixel(point.y, point.x, rgb);

	strcat_s(xy, sizeof(xy),rgb);

	WCHAR str[100];
	wsprintf(str, L"%S", xy);
	((CMainFrame*)GetParent())->SetMessageText(str);

	CScrollView::OnMouseMove(nFlags, point);
}

extern BITMAPINFO* lpBitsInfoOrigin;
extern DWORD bmOriginSize;
void CCFD188View::OnReload()
{
	free(lpBitsInfo);
	lpBitsInfo = (BITMAPINFO*)malloc(bmOriginSize);
	memcpy(lpBitsInfo, lpBitsInfoOrigin, bmOriginSize);
	Invalidate();
}


void CCFD188View::OnUpdateReload(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(lpBitsInfoOrigin != NULL);
}

#include "CDlgHistogram.h"
void CCFD188View::OnHistogram()
{
	CDlgHistogram dlg;
	dlg.DoModal();
}


void CCFD188View::OnUpdateHistogram(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(lpBitsInfoOrigin != NULL);
}
