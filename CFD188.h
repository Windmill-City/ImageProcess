
// CFD188.h: CFD188 应用程序的主头文件
//
#pragma once
#include "ImageFormats.h"

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号


// CCFD188App:
// 有关此类的实现，请参阅 CFD188.cpp
//

class CCFD188App : public CWinApp
{
public:
	std::unique_ptr<ImageFormats> Formats = std::make_unique<ImageFormats>();
public:
	CCFD188App() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCFD188App theApp;
