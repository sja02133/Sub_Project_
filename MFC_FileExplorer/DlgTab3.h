#pragma once


// cDlgTab3 dialog

class cDlgTab3 : public CDialogEx
{
	DECLARE_DYNAMIC(cDlgTab3)

public:
	cDlgTab3(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cDlgTab3();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
