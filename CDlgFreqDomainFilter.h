#pragma once

#include <afxdialogex.h>

#include "CFD188View.h"
#include "ImageFilterTransform.h"

// CDlgFreqDomainFilter 对话框

class CDlgFreqDomainFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFreqDomainFilter)
private:
	CEdit EditDistance;
	CComboBox FilterType;

	CCFD188View* View = nullptr;

	std::shared_ptr<Image> ImageOriginal;
	std::unique_ptr<ImageFreqDomainFilterContext> ActiveFilterContext;

public:
	CDlgFreqDomainFilter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgFreqDomainFilter();

	void setView(CCFD188View* pView);
	void onFilterContextChange();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FD_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeDistance();
	afx_msg void OnCbnSelchangeType();
	CEdit EditN;
	afx_msg void OnPaint();
};
