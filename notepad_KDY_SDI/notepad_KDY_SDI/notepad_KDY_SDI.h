
// notepad_KDY_SDI.h : main header file for the notepad_KDY_SDI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CnotepadKDYSDIApp:
// See notepad_KDY_SDI.cpp for the implementation of this class
//

class CnotepadKDYSDIApp : public CWinApp
{
public:
	CnotepadKDYSDIApp() noexcept;

	int id;
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMenuSaveAs();
	afx_msg void OnUpdateMenuSaveAs(CCmdUI* pCmdUI);
};

extern CnotepadKDYSDIApp theApp;
