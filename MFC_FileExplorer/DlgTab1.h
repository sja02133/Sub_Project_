#pragma once


// cDlgTab1 dialog

class cDlgTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(cDlgTab1)

public:
	cDlgTab1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~cDlgTab1();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
