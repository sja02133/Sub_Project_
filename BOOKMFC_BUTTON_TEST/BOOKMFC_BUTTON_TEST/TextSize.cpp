// TextSize.cpp : implementation file
//

#include "pch.h"

// cTextSize dialog

IMPLEMENT_DYNAMIC(cTextSize, CDialogEx)

cTextSize::cTextSize(CWnd* pParent /*=nullptr*/)
	: cDlgPaint(pParent, IDD_DIALOG_CHANGE_CONFIG_TEXT_SIZE)
{

}

cTextSize::~cTextSize()
{
}

void cTextSize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cTextSize, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEXT_SIZE_UP, &cTextSize::OnBnClickedButtonTextSizeUp)
	ON_BN_CLICKED(IDC_BUTTON_TEXT_SIZE_DOWN, &cTextSize::OnBnClickedButtonTextSizeDown)
	ON_BN_CLICKED(IDC_BUTTON_TEXT_SIZE_COLLECT, &cTextSize::OnBnClickedButtonTextSizeCollect)
END_MESSAGE_MAP()


// cTextSize message handlers



//void cTextSize::OnInitMenu(CMenu* pMenu)
//{
//	CDialogEx::OnInitMenu(pMenu);
//
//	// TODO: Add your message handler code here
//}

BOOL cTextSize::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	str.Format(L"%d", theData.set_thickness.thickness);
	SetDlgItemText(IDC_STATIC_TEXT_SIZE,str);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void cTextSize::OnBnClickedButtonTextSizeUp()
{
	CString str;
	unsigned int thickness_num;
	GetDlgItemText(IDC_STATIC_TEXT_SIZE, str);
	thickness_num = _ttoi(str);

	if (thickness_num > 100)
		MessageBox(L"최대 100까지만 설정 가능");
	else
		thickness_num++;

	str.Format(L"%d", thickness_num);
	SetDlgItemText(IDC_STATIC_TEXT_SIZE, str);

	// TODO: Add your control notification handler code here
}


void cTextSize::OnBnClickedButtonTextSizeDown()
{
	CString str;
	unsigned int thickness_num;
	GetDlgItemText(IDC_STATIC_TEXT_SIZE, str);
	thickness_num = _ttoi(str);

	if (thickness_num <= 1)
		MessageBox(L"최소 1까지만 설정 가능");
	else
		thickness_num--;

	str.Format(L"%d", thickness_num);
	SetDlgItemText(IDC_STATIC_TEXT_SIZE, str);

	// TODO: Add your control notification handler code here
}


void cTextSize::OnBnClickedButtonTextSizeCollect()
{
	CString thickness_str;

	GetDlgItemText(IDC_STATIC_TEXT_SIZE, thickness_str);

	theData.set_thickness.thickness = _ttoi(thickness_str);
	
	CDialogEx::OnOK();
	// TODO: Add your control notification handler code here
}
