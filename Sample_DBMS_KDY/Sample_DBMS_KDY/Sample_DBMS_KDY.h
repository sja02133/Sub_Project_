
// Sample_DBMS_KDY.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

typedef struct dbConnInfo {
	CString name;
	CString password;
	CString ip;
	CString port;
	CString database;
}DbConnInfo;

// CSampleDBMSKDYApp:
// See Sample_DBMS_KDY.cpp for the implementation of this class
//

class CSampleDBMSKDYApp : public CWinApp
{
public:
	CSampleDBMSKDYApp();

	BOOL odbcCheck = TRUE;
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSampleDBMSKDYApp theApp;


void connectDB(DbConnInfo* info);