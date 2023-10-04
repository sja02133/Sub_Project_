// MenuTab.cpp : implementation file
//

#include "pch.h"
#include "MFC_FileExplorer.h"
#include "MenuTab.h"
#include "afxdialogex.h"


// cMenuTab dialog

IMPLEMENT_DYNAMIC(cMenuTab, CDialogEx)

cMenuTab::cMenuTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(fileExplorer, pParent)
{

}

cMenuTab::~cMenuTab()
{
}

void cMenuTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON4, fixedButton);
}


BEGIN_MESSAGE_MAP(cMenuTab, CDialogEx)
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &cMenuTab::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// cMenuTab message handlers

//void cMenuTab::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//
//
//}

