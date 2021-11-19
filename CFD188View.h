
// CFD188View.h: CCFD188View 类的接口
//

#pragma once

#include "CFD188Doc.h"

class CCFD188View : public CScrollView
{
protected: // 仅从序列化创建
	CCFD188View() noexcept;
	DECLARE_DYNCREATE(CCFD188View)

// 特性
public:
	CCFD188Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCFD188View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGrey();
	afx_msg void OnUpdateGrey(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnReload();
	afx_msg void OnUpdateReload(CCmdUI* pCmdUI);
	afx_msg void OnHistogram();
	afx_msg void OnUpdateHistogram(CCmdUI* pCmdUI);
	afx_msg void OnEqualize();
	afx_msg void OnUpdateEqualize(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // CFD188View.cpp 中的调试版本
inline CCFD188Doc* CCFD188View::GetDocument() const
   { return reinterpret_cast<CCFD188Doc*>(m_pDocument); }
#endif

