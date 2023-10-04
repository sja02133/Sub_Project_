
// winpcap_first_KDY.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "lanCardInfo.h"
#include "cWinPcapKDY.h"
#include "SplitterControl.h"
#include "DlgMonitoring.h"

#include "fileInputOutput.h"	//2023.02.06 Ãß°¡
// CwinpcapfirstKDYApp:
// See winpcap_first_KDY.cpp for the implementation of this class
//

/*
struct MyNet{
	UINT type;
};

struct MyTcp : public MyNet{
	char data;
};

struct MyUcp : public MyNet {
	char data;
};

struct MyArp : public MyNet {
	char data;
};
*/
class CwinpcapfirstKDYApp : public CWinApp,
	public lanCardInfo,
	public cWinPcapKDY
{
public:
	CwinpcapfirstKDYApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	lanCardInfo lanInfo;
	cWinPcapKDY pcap_kdy;
	DECLARE_MESSAGE_MAP()

//	std::list<MyNet*> list;

};

extern CwinpcapfirstKDYApp theApp;