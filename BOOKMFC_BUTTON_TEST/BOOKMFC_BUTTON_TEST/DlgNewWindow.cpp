// DlgNewWindow.cpp : implementation file
//

#include "pch.h"


// cDlgNewWindow dialog

IMPLEMENT_DYNAMIC(cDlgNewWindow, CDialogEx)

cDlgNewWindow::cDlgNewWindow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NEW_WINDOW, pParent)
	, static_string(_T(""))
{
}

cDlgNewWindow::~cDlgNewWindow()
{
}

void cDlgNewWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NEW_WINDOW, controlNewWindow);
	DDX_Text(pDX, IDC_STATIC_NEW_WINDOW, static_string);
}


BEGIN_MESSAGE_MAP(cDlgNewWindow, CDialogEx)
END_MESSAGE_MAP()


// cDlgNewWindow message handlers
