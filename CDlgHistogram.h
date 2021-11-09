#pragma once


// CDlgHistogram 对话框

class CDlgHistogram : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHistogram)

public:
	CDlgHistogram(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgHistogram();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Histogram };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
