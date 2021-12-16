#pragma once

#include <afxdialogex.h>
#include <vector>

#include "CFD188View.h"

// CDlgHistogram 对话框

class CDlgHistogram : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHistogram)
private:
	CSliderCtrl PointLeft;
	CSliderCtrl PointRight;
	CComboBox ChannelSelector;

	CCFD188View* View = nullptr;

	std::shared_ptr<Image> ImageOriginal;
	std::vector<size_t> Histogram;

	int PosLeft = 0;
	int PosRight = 0;
public:
	CDlgHistogram(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgHistogram();
	
	void setView(CCFD188View* pView);
	void applyLinearTrans();
	void refreshHistogram();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Histogram };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeChannelSelect();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
