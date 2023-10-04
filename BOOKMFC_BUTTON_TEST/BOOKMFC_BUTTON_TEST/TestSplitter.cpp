// TestSplitter.cpp : implementation file
//

#include "pch.h"
#include "BOOKMFC_BUTTON_TEST.h"
#include "TestSplitter.h"
#include "afxdialogex.h"


// cTestSplitter dialog

IMPLEMENT_DYNAMIC(cTestSplitter, CDialogEx)

cTestSplitter::cTestSplitter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TEST_SPLITTER, pParent)
{

}

cTestSplitter::~cTestSplitter()
{
}

void cTestSplitter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cTestSplitter, CDialogEx)
END_MESSAGE_MAP()


// cTestSplitter message handlers


BOOL cTestSplitter::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}
