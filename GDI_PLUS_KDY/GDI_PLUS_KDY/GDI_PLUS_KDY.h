
// GDI_PLUS_KDY.h : main header file for the GDI_PLUS_KDY application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGDIPLUSKDYApp:
// See GDI_PLUS_KDY.cpp for the implementation of this class
//

class CGDIPLUSKDYApp : public CWinApp
{
public:
	CGDIPLUSKDYApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


public:
	int select_figure = -1;
public:
	void changeButtonColor(int id);

};

extern CGDIPLUSKDYApp theApp;

