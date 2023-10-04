#pragma once


// cDlgTab2 dialog

class cDlgTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(cDlgTab2)

public:
	cDlgTab2(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cDlgTab2();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
