// CDlgFreqDomainFilter.cpp: 实现文件
//

#include "pch.h"
#include "CFD188.h"
#include "CDlgFreqDomainFilter.h"
#include "afxdialogex.h"

// CDlgFreqDomainFilter 对话框

IMPLEMENT_DYNAMIC(CDlgFreqDomainFilter, CDialogEx)

CDlgFreqDomainFilter::CDlgFreqDomainFilter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FD_FILTER, pParent)
{

}

CDlgFreqDomainFilter::~CDlgFreqDomainFilter()
{
}

void CDlgFreqDomainFilter::setView(CCFD188View* pView)
{
	View = pView;
	ImageOriginal = std::make_shared<Image>(*View->GetDocument()->ActiveImage);
}

void CDlgFreqDomainFilter::onFilterContextChange()
{
	auto width = ImageOriginal->Width;
	auto height = ImageOriginal->Height;

	CString strD0;
	GetDlgItemText(IDC_Distance, strD0);
	auto d0 = _ttoi(strD0);

	CString strN;
	GetDlgItemText(IDC_N, strN);
	auto n = _ttoi(strN);

	auto type = FilterType.GetCurSel() % 2 == 0 ? FilterType::LowPass : FilterType::HighPass;

	//Ideal
	if (FilterType.GetCurSel() < 2) {
		EditN.EnableWindow(false);
		ActiveFilterContext = std::make_unique<IdealFilterContext>(type, width, height, d0);
	}
	//Butterworth
	else {
		EditN.EnableWindow(true);
		ActiveFilterContext = std::make_unique<ButterworthFilterContext>(n, type, width, height, d0);
	}

	View->GetDocument()->ActiveImage = ImageFreqDomainFilter::applyFreqDomainFilter(ImageOriginal, *ActiveFilterContext);
	View->GetDocument()->SetModifiedFlag();
	View->Invalidate();
	Invalidate();
}

void CDlgFreqDomainFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Distance, EditDistance);
	DDX_Control(pDX, IDC_Type, FilterType);
	DDX_Control(pDX, IDC_N, EditN);

	FilterType.InsertString(0, L"Ideal Lowpass");
	FilterType.InsertString(1, L"Ideal Highpass");
	FilterType.InsertString(2, L"Butterworth Lowpass");
	FilterType.InsertString(3, L"Butterworth Highpass");

	FilterType.SetCurSel(0);
}


BEGIN_MESSAGE_MAP(CDlgFreqDomainFilter, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CDlgFreqDomainFilter::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_Distance, &CDlgFreqDomainFilter::OnEnChangeDistance)
	ON_EN_CHANGE(IDC_N, &CDlgFreqDomainFilter::OnEnChangeDistance)
	ON_CBN_SELCHANGE(IDC_Type, &CDlgFreqDomainFilter::OnCbnSelchangeType)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgFreqDomainFilter 消息处理程序


void CDlgFreqDomainFilter::OnBnClickedCancel()
{
	View->GetDocument()->ActiveImage = ImageOriginal;
	View->Invalidate();
	CDialogEx::OnCancel();
}

void CDlgFreqDomainFilter::OnEnChangeDistance()
{
	onFilterContextChange();
}


void CDlgFreqDomainFilter::OnCbnSelchangeType()
{
	onFilterContextChange();
}


void CDlgFreqDomainFilter::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (!ActiveFilterContext) return;
	auto image = ActiveFilterContext->getFilterImage();

	//内存DC, 双缓冲[内存DC-设备DC]
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);

	//绘图区域
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, (int)image->Width, (int)image->Height);

	//绘图区域属性
	BITMAP bm;
	bitmap.GetObject(sizeof(bm), &bm);

	//图像数据->绘图区域
	bitmap.SetBitmapBits(bm.bmWidthBytes * bm.bmHeight, image->Pixels.data());

	//绘图区域->内存DC, 在内存中绘制
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);

	//绘制区域->设备DC, 显示绘制结果
	dc.StretchBlt(250, 100, 128, 128, &memDC, 0, 0, image->Width, image->Height, SRCCOPY);

	//释放位图对象, 避免资源泄露
	memDC.SelectObject(pOldBmp);
}
