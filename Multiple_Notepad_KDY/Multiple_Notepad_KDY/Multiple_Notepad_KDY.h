
// Multiple_Notepad_KDY.h : main header file for the Multiple_Notepad_KDY application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "FileDialogCustom.h"

// CMultipleNotepadKDYApp:
// See Multiple_Notepad_KDY.cpp for the implementation of this class
//

class CMultipleNotepadKDYApp : public CWinApp
{
public:
	CMultipleNotepadKDYApp() noexcept;

	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();

//	BOOL OnClose();
//	BOOL settingFileInformation(CString fullFileString, eBOM bom);

	CDocument* pDoc;
	CView* pView;
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMultipleNotepadKDYApp theApp;