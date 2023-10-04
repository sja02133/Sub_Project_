// TreeControl.cpp : implementation file
//

#include "pch.h"
#include "MFC_FileExplorer.h"
#include "TreeControl.h"
#include "afxdialogex.h"


// cTreeControl dialog

IMPLEMENT_DYNAMIC(cTreeControl, CDialogEx)

cTreeControl::cTreeControl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(fileExplorer, pParent)
{

}

cTreeControl::~cTreeControl()
{
}

void cTreeControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cTreeControl, CDialogEx)
END_MESSAGE_MAP()


// cTreeControl message handlers


BOOL cTreeControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HTREEITEM root, hChild1, hChild2;

	CTreeCtrl c_tree;

	CString cst = L"aaa";

	//root = c_tree.InsertItem(cst, 0, 0, TVI_ROOT, TVI_LAST);



	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
