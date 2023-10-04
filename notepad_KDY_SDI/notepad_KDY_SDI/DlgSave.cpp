// DlgSave.cpp : implementation file
//

#include "pch.h"
#include "notepad_KDY_SDI.h"
#include "DlgSave.h"
#include "afxdialogex.h"


// cDlgSave dialog

IMPLEMENT_DYNAMIC(cDlgSave, CDialogEx)

cDlgSave::cDlgSave(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SAVE, pParent)
{

}

cDlgSave::~cDlgSave()
{
}

void cDlgSave::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDlgSave, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &cDlgSave::OnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_NONE_SAVE, &cDlgSave::OnClickedButtonNoneSave)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &cDlgSave::OnClickedButtonCancel)
END_MESSAGE_MAP()


// cDlgSave message handlers


BOOL cDlgSave::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	CFont font;
	LOGFONT lf;

	lf.lfHeight = 15;
	lf.lfWeight = FW_BOLD;
	
	font.CreateFontIndirectW(&lf);

	SetDlgItemText(IDC_STATIC_SAVE_STR, L"¸Þ¸ðÀå");
	GetDlgItem(IDC_STATIC_SAVE_STR)->SetFont(&font, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void cDlgSave::OnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	
	theApp.id = 1;
	OnOK();
}


void cDlgSave::OnClickedButtonNoneSave()
{
	// TODO: Add your control notification handler code here
	theApp.id = 2;
	OnOK();
}


void cDlgSave::OnClickedButtonCancel()
{
	// TODO: Add your control notification handler code here
	theApp.id = 0;
	OnOK();
}
