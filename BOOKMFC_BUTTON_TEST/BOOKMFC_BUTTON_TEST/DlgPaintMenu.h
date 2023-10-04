#pragma once


// cDlgPaintMenu dialog

class cDlgPaintMenu : public CDialogEx
{
	DECLARE_DYNAMIC(cDlgPaintMenu)

public:
	cDlgPaintMenu(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cDlgPaintMenu();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAINT_MENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
