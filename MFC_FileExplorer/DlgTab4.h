#pragma once


// cDlgTab4 dialog

class cDlgTab4 : public CDialogEx
{
	DECLARE_DYNAMIC(cDlgTab4)

public:
	cDlgTab4(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cDlgTab4();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
