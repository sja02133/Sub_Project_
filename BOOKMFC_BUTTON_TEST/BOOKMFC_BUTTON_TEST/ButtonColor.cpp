// ButtonColorBlack.cpp : implementation file
//

#include "pch.h"


// cButtonColor dialog

IMPLEMENT_DYNAMIC(cButtonColor, CDialogEx)

cButtonColor::cButtonColor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHANGE_CONFIG_COLOR, pParent)
{

}

cButtonColor::~cButtonColor()
{
}

void cButtonColor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cButtonColor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_RED, &cButtonColor::OnBnClickedButtonColorRed)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_BLACK, &cButtonColor::OnBnClickedButtonColorBlack)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_YELLOW, &cButtonColor::OnBnClickedButtonColorYellow)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_GREEN, &cButtonColor::OnBnClickedButtonColorGreen)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_ORANGE, &cButtonColor::OnBnClickedButtonColorOrange)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// cButtonColor message handlers
cButtonColor buttonColor;
void cButtonColor::OnBnClickedButtonColorRed()
{
	theData.set_color.red = 255;
	theData.set_color.green = 0;
	theData.set_color.blue = 0;

	CDialogEx::OnOK();
	// TODO: Add your control notification handler code here
}


void cButtonColor::OnBnClickedButtonColorBlack()
{
	theData.set_color.red = 0;
	theData.set_color.green = 0;
	theData.set_color.blue = 0;
	
	CDialogEx::OnOK();
	// TODO: Add your control notification handler code here
}


void cButtonColor::OnBnClickedButtonColorYellow()
{
	theData.set_color.red = 255;
	theData.set_color.green = 255;
	theData.set_color.blue = 0;

	CDialogEx::OnOK();
	// TODO: Add your control notification handler code here
}


void cButtonColor::OnBnClickedButtonColorGreen()
{
	theData.set_color.red = 0;
	theData.set_color.green = 255;
	theData.set_color.blue = 0;

	CDialogEx::OnOK();
	// TODO: Add your control notification handler code here
}


void cButtonColor::OnBnClickedButtonColorOrange()
{
	theData.set_color.red = 255;
	theData.set_color.green = 100;
	theData.set_color.blue = 0;

	CDialogEx::OnOK();
	// TODO: Add your control notification handler code here
}

penColor::penColor() {
	red = 0;
	green = 0;
	blue = 0;
}

penThickness::penThickness() {
	thickness = 1;
}

void cButtonColor::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
	CDC dc;
	RECT rect;
	dc.Attach(lpDrawItemStruct->hDC);
	rect = lpDrawItemStruct->rcItem;

	switch (nIDCtl) {
	case IDC_BUTTON_COLOR_BLACK:
		dc.FillSolidRect(&rect, RGB(0, 0, 0));
		break;
	case IDC_BUTTON_COLOR_RED:
		dc.FillSolidRect(&rect, RGB(255, 0, 0));
		break;
	case IDC_BUTTON_COLOR_YELLOW:
		dc.FillSolidRect(&rect, RGB(255, 255, 0));
		break; 
	case IDC_BUTTON_COLOR_ORANGE:
		dc.FillSolidRect(&rect, RGB(255, 100, 0));
		break;
	case IDC_BUTTON_COLOR_GREEN:
		dc.FillSolidRect(&rect, RGB(0, 255, 0));
		break;
	}
}
